
#include "nRF24L01.h"
#include "RF24_config.h"
#include "RF24.h"

/****************************************************************************/

  uint16_t ce_pin; /**< "Chip Enable" pin, activates the RX or TX role */
  uint16_t csn_pin; /**< SPI Chip select */
  uint16_t spi_speed; /**< SPI Bus Speed */
  uint8_t p_variant; /* False for RF24L01 and true for RF24L01P */
  uint8_t payload_size; /**< Fixed size of payloads */
  uint8_t dynamic_payloads_enabled; /**< Whether dynamic payloads are enabled. */
  uint8_t pipe0_reading_address[5]; /**< Last address set on pipe 0 for reading. */
  uint8_t addr_width; /**< The address width to use - 3,4 or 5 bytes. */


  uint32_t txDelay;
  uint32_t csDelay;


void csn(uint8_t mode)
{
  digitalWrite(csn_pin,mode);
  delayMicroseconds(csDelay);
}

/****************************************************************************/

void ce(uint8_t level)
{
  digitalWrite(ce_pin,level);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
uint8_t read_register(uint8_t reg)
{
  uint8_t result;

  csn(LOW);

  spi_transfer( R_REGISTER | ( REGISTER_MASK & reg ) );
  result = spi_transfer(0xff);

  csn(HIGH);

  return result;
}

/****************************************************************************/

uint8_t write_registers(uint8_t reg, const uint8_t* buf, uint8_t len)
{
  uint8_t status;

  csn(LOW);

  status = spi_transfer( W_REGISTER | ( REGISTER_MASK & reg ) );
  while ( len-- )
    spi_transfer(*buf++);

  csn(HIGH);

  return status;
}

/****************************************************************************/

uint8_t write_register(uint8_t reg, uint8_t value)
{
  uint8_t status;

  csn(LOW);

  status = spi_transfer( W_REGISTER | ( REGISTER_MASK & reg ) );
  spi_transfer(value);

  csn(HIGH);

  return status;
}

/****************************************************************************/

uint8_t write_payload(const void* buf, uint8_t data_len, const uint8_t writeType)
{
  uint8_t status;
  const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);

   data_len = rf24_min(data_len, payload_size);
   uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;

  csn(LOW);

  status = spi_transfer( writeType );
  while ( data_len-- ) {
    spi_transfer(*current++);
  }
  while ( blank_len-- ) {
    spi_transfer(0);
  }

  csn(HIGH);

  return status;
}

/****************************************************************************/

uint8_t read_payload(void* buf, uint8_t data_len)
{
  uint8_t status;
  uint8_t* current = reinterpret_cast<uint8_t*>(buf);

  if(data_len > payload_size) data_len = payload_size;
  uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;


  csn(LOW);

  status = spi_transfer( R_RX_PAYLOAD );
  while ( data_len-- ) {
    *current++ = spi_transfer(0xFF);
  }
  while ( blank_len-- ) {
    spi_transfer(0xff);
  }

  csn(HIGH);

  return status;
}

/****************************************************************************/

uint8_t flush_rx(void)
{
  return spiTrans( FLUSH_RX );
}

/****************************************************************************/

uint8_t flush_tx(void)
{
  return spiTrans( FLUSH_TX );
}

/****************************************************************************/

uint8_t spiTrans(uint8_t cmd){

  uint8_t status;

  csn(LOW);
  status = spi_transfer( cmd );
  csn(HIGH);

  return status;
}

/****************************************************************************/

uint8_t get_status(void)
{
  return spiTrans(RF24_NOP);
}

/****************************************************************************/
/****************************************************************************/

void setChannel(uint8_t channel)
{
  const uint8_t max_channel = 125;
  write_register(RF_CH,rf24_min(channel,max_channel));
}

uint8_t getChannel()
{

  return read_register(RF_CH);
}
/****************************************************************************/

void setPayloadSize(uint8_t size)
{
  payload_size = rf24_min(size,32);
}

/****************************************************************************/

uint8_t getPayloadSize(void)
{
  return payload_size;
}

/****************************************************************************/
/****************************************************************************/

uint8_t begin(void)
{

//RF24::RF24(uint16_t _cepin, uint16_t _cspin):
//  ce_pin(_cepin), csn_pin(_cspin),

  p_variant = false;
  payload_size = 32;
  dynamic_payloads_enabled = false;
  addr_width =5 ;
  csDelay = 5  //,pipe0_reading_address(0)

  pipe0_reading_address[0]=0;

  uint8_t setup=0;

    // Initialize pins
    pinMode(ce_pin,OUTPUT);

    pinMode(csn_pin,OUTPUT);

//    _SPI.begin();
    ce(LOW);
    csn(HIGH);

  // Must allow the radio time to settle else configuration bits will not necessarily stick.
  // This is actually only required following power up but some settling time also appears to
  // be required after resets too. For full coverage, we'll always assume the worst.
  // Enabling 16b CRC is by far the most obvious case if the wrong timing is used - or skipped.
  // Technically we require 4.5ms + 14us as a worst case. We'll just call it 5ms for good measure.
  // WARNING: Delay is based on P-variant whereby non-P *may* require different timing.
  delay( 5 ) ;

  // Reset NRF_CONFIG and enable 16-bit CRC.
  write_register( NRF_CONFIG, 0x0C ) ;

  // Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
  // WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
  // sizes must never be used. See documentation for a more complete explanation.
  setRetries(5,15);

  // Reset value is MAX
  //setPALevel( RF24_PA_MAX ) ;

  // check for connected module and if this is a p nRF24l01 variant
  //
  if( setDataRate( RF24_250KBPS ) )
  {
    p_variant = true ;
  }
  setup = read_register(RF_SETUP);
  /*if( setup == 0b00001110 )     // register default for nRF24L01P
  {
    p_variant = true ;
  }*/

  // Then set the data rate to the slowest (and most reliable) speed supported by all
  // hardware.
  setDataRate( RF24_1MBPS ) ;

  // Initialize CRC and request 2-byte (16bit) CRC
  //setCRCLength( RF24_CRC_16 ) ;

  // Disable dynamic payloads, to match dynamic_payloads_enabled setting - Reset value is 0
  toggle_features();
  write_register(FEATURE,0 );
  write_register(DYNPD,0);
  dynamic_payloads_enabled = false;

  // Reset current status
  // Notice reset and flush is the last thing we do
  write_register(NRF_STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

  // Set up default configuration.  Callers can always change it later.
  // This channel should be universally safe and not bleed over into adjacent
  // spectrum.
  setChannel(76);

  // Flush buffers
  flush_rx();
  flush_tx();

  powerUp(); //Power up by default when begin() is called

  // Enable PTX, do not write CE high so radio will remain in standby I mode ( 130us max to transition to RX or TX instead of 1500us from powerUp )
  // PTX should use only 22uA of power
  write_register(NRF_CONFIG, ( read_register(NRF_CONFIG) ) & ~_BV(PRIM_RX) );

  // if setup is 0 or ff then there was no response from module
  return ( setup != 0 && setup != 0xff );
}

/****************************************************************************/

uint8_t isChipConnected()
{
  uint8_t setup = read_register(SETUP_AW);
  if(setup >= 1 && setup <= 3)
  {
    return true;
  }

  return false;
}

/****************************************************************************/

void startListening(void)
{
  powerUp();
  write_register(NRF_CONFIG, read_register(NRF_CONFIG) | _BV(PRIM_RX));
  write_register(NRF_STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );
  ce(HIGH);
  // Restore the pipe0 adddress, if exists
  if (pipe0_reading_address[0] > 0){
    write_registers(RX_ADDR_P0, pipe0_reading_address, addr_width);
  }else{
        closeReadingPipe(0);
  }

  // Flush buffers
  //flush_rx();
  if(read_register(FEATURE) & _BV(EN_ACK_PAY)){
        flush_tx();
  }

  // Go!
  //delayMicroseconds(100);
}

/****************************************************************************/
static const uint8_t child_pipe_enable[] =
{
  ERX_P0, ERX_P1, ERX_P2, ERX_P3, ERX_P4, ERX_P5
};

void stopListening(void)
{
  ce(LOW);

  delayMicroseconds(txDelay);

  if(read_register(FEATURE) & _BV(EN_ACK_PAY)){
    delayMicroseconds(txDelay); //200
        flush_tx();
  }
  //flush_rx();
  write_register(NRF_CONFIG, ( read_register(NRF_CONFIG) ) & ~_BV(PRIM_RX) );

  write_register(EN_RXADDR,read_register(EN_RXADDR) | _BV(child_pipe_enable[0])); // Enable RX on pipe0

  //delayMicroseconds(100);

}

/****************************************************************************/

void powerDown(void)
{
  ce(LOW); // Guarantee CE is low on powerDown
  write_register(NRF_CONFIG,read_register(NRF_CONFIG) & ~_BV(PWR_UP));
}

/****************************************************************************/

//Power up now. Radio will not power down unless instructed by MCU for config changes etc.
void powerUp(void)
{
   uint8_t cfg = read_register(NRF_CONFIG);

   // if not powered up then power up and wait for the radio to initialize
   if (!(cfg & _BV(PWR_UP))){
      write_register(NRF_CONFIG, cfg | _BV(PWR_UP));

      // For nRF24L01+ to go from power down mode to TX or RX mode it must first pass through stand-by mode.
          // There must be a delay of Tpd2stby (see Table 16.) after the nRF24L01+ leaves power down mode before
          // the CEis set high. - Tpd2stby can be up to 5ms per the 1.0 datasheet
      delay(5);
   }
}

/******************************************************************/
/******************************************************************/

//Similar to the previous write, clears the interrupt flags
uint8_t write( const void* buf, uint8_t len, const uint8_t multicast )
{
        //Start Writing
        startFastWrite(buf,len,multicast);

        //Wait until complete or failed

        while( ! ( get_status()  & ( _BV(TX_DS) | _BV(MAX_RT) ))) {
        }

        ce(LOW);

        uint8_t status = write_register(NRF_STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

  //Max retries exceeded
  if( status & _BV(MAX_RT)){
        flush_tx(); //Only going to be 1 packet int the FIFO at a time using this method, so just flush
        return 0;
  }
        //TX OK 1 or 0
  return 1;
}

//uint8_t RF24::write( const void* buf, uint8_t len ){
//        return write(buf,len,0);
//}
/****************************************************************************/

//For general use, the interrupt flags are not important to clear
uint8_t writeBlocking( const void* buf, uint8_t len, uint32_t timeout )
{
        //Block until the FIFO is NOT full.
        //Keep track of the MAX retries and set auto-retry if seeing failures
        //This way the FIFO will fill up and allow blocking until packets go through
        //The radio will auto-clear everything in the FIFO as long as CE remains high

        uint32_t timer = millis();                                                        //Get the time that the payload transmission started

        while( ( get_status()  & ( _BV(TX_FULL) ))) {             //Blocking only if FIFO is full. This will loop and block until TX is successful or timeout

                if( get_status() & _BV(MAX_RT)){                                          //If MAX Retries have been reached
                        reUseTX();                                                                                //Set re-transmit and clear the MAX_RT interrupt flag
                        if(millis() - timer > timeout){ return 0; }               //If this payload has exceeded the user-defined timeout, exit and return 0
                }
        }

        //Start Writing
        startFastWrite(buf,len,0);                                                                //Write the payload if a buffer is clear

        return 1;                                                                                                 //Return 1 to indicate successful transmission
}

/****************************************************************************/

void reUseTX(){
                write_register(NRF_STATUS,_BV(MAX_RT) );                          //Clear max retry flag
                spiTrans( REUSE_TX_PL );
                ce(LOW);                                                                                  //Re-Transfer packet
                ce(HIGH);
}

/****************************************************************************/

uint8_t writeFast( const void* buf, uint8_t len, const uint8_t multicast )
{
        //Block until the FIFO is NOT full.
        //Keep track of the MAX retries and set auto-retry if seeing failures
        //Return 0 so the user can control the retrys and set a timer or failure counter if required
        //The radio will auto-clear everything in the FIFO as long as CE remains high


        while( ( get_status()  & ( _BV(TX_FULL) ))) {                     //Blocking only if FIFO is full. This will loop and block until TX is successful or fail

                if( get_status() & _BV(MAX_RT)){
                        //reUseTX();                                                                              //Set re-transmit
                        write_register(NRF_STATUS,_BV(MAX_RT) );                          //Clear max retry flag
                        return 0;                                                                                 //Return 0. The previous payload has been retransmitted
                                                                                                                          //From the user perspective, if you get a 0, just keep trying to send the same payload
                }
        }
                     //Start Writing
        startFastWrite(buf,len,multicast);

        return 1;
}


//uint8_t RF24::writeFast( const void* buf, uint8_t len ){
//        return writeFast(buf,len,0);
//}

/****************************************************************************/

//Per the documentation, we want to set PTX Mode when not listening. Then all we do is write data and set CE high
//In this mode, if we can keep the FIFO buffers loaded, packets will transmit immediately (no 130us delay)
//Otherwise we enter Standby-II mode, which is still faster than standby mode
//Also, we remove the need to keep writing the config register over and over and delaying for 150 us each time if sending a stream of data

void startFastWrite( const void* buf, uint8_t len, const uint8_t multicast, uint8_t startTx){ //TMRh20

        //write_payload( buf,len);
        write_payload( buf, len,multicast ? W_TX_PAYLOAD_NO_ACK : W_TX_PAYLOAD ) ;
        if(startTx){
                ce(HIGH);
        }

}

/****************************************************************************/

//Added the original startWrite back in so users can still use interrupts, ack payloads, etc
//Allows the library to pass all tests
void startWrite( const void* buf, uint8_t len, const uint8_t multicast ){

  // Send the payload

  //write_payload( buf, len );
  write_payload( buf, len,multicast? W_TX_PAYLOAD_NO_ACK : W_TX_PAYLOAD ) ;
  ce(HIGH);
  #if defined(CORE_TEENSY) || !defined(ARDUINO) || defined (RF24_SPIDEV) || defined (RF24_DUE)
        delayMicroseconds(10);
  #endif
  ce(LOW);


}

/****************************************************************************/

uint8_t rxFifoFull(){
        return read_register(FIFO_STATUS) & _BV(RX_FULL);
}
/****************************************************************************/

uint8_t txStandBy(){

        while( ! (read_register(FIFO_STATUS) & _BV(TX_EMPTY)) ){
                if( get_status() & _BV(MAX_RT)){
                        write_register(NRF_STATUS,_BV(MAX_RT) );
                        ce(LOW);
                        flush_tx();    //Non blocking, flush the data
                        return 0;
                }
        }

        ce(LOW);                           //Set STANDBY-I mode
        return 1;
}

/****************************************************************************/

uint8_t txStandBy(uint32_t timeout, uint8_t startTx){

    if(startTx){
          stopListening();
          ce(HIGH);
        }
        uint32_t start = millis();

        while( ! (read_register(FIFO_STATUS) & _BV(TX_EMPTY)) ){
                if( get_status() & _BV(MAX_RT)){
                        write_register(NRF_STATUS,_BV(MAX_RT) );
                                ce(LOW);                                                                                  //Set re-transmit
                                ce(HIGH);
                                if(millis() - start >= timeout){
                                        ce(LOW); flush_tx(); return 0;
                                }
                }
        }


        ce(LOW);                                   //Set STANDBY-I mode
        return 1;

}

/****************************************************************************/

void maskIRQ(uint8_t tx, uint8_t fail, uint8_t rx){

        uint8_t config = read_register(NRF_CONFIG);
        /* clear the interrupt flags */
        config &= ~(1 << MASK_MAX_RT | 1 << MASK_TX_DS | 1 << MASK_RX_DR);
        /* set the specified interrupt flags */
        config |= fail << MASK_MAX_RT | tx << MASK_TX_DS | rx << MASK_RX_DR;
        write_register(NRF_CONFIG, config);
}

/****************************************************************************/

uint8_t getDynamicPayloadSize(void)
{
  uint8_t result = 0;

  csn(LOW);
  spi_transfer( R_RX_PL_WID );
  result = spi_transfer(0xff);
  csn(HIGH);

  if(result > 32) { flush_rx(); delay(2); return 0; }
  return result;
}

/****************************************************************************/

//uint8_t available(void)
//{
//  return available(NULL);
//}

/****************************************************************************/

uint8_t available(uint8_t* pipe_num)
{
  if (!( read_register(FIFO_STATUS) & _BV(RX_EMPTY) )){

    // If the caller wants the pipe number, include that
    if ( pipe_num ){
          uint8_t status = get_status();
      *pipe_num = ( status >> RX_P_NO ) & 0x07;
        }
        return 1;
  }


  return 0;


}

/****************************************************************************/

void read( void* buf, uint8_t len ){

  // Fetch the payload
  read_payload( buf, len );

  //Clear the two possible interrupt flags with one command
  write_register(NRF_STATUS,_BV(RX_DR) | _BV(MAX_RT) | _BV(TX_DS) );

}

/****************************************************************************/

void whatHappened(uint8_t& tx_ok,uint8_t& tx_fail,uint8_t& rx_ready)
{
  // Read the status & reset the status in one easy call
  // Or is that such a good idea?
  uint8_t status = write_register(NRF_STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

  // Report to the user what happened
  tx_ok = status & _BV(TX_DS);
  tx_fail = status & _BV(MAX_RT);
  rx_ready = status & _BV(RX_DR);
}

/****************************************************************************/

void openWritingPipe(uint64_t value)
{
  // Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
  // expects it LSB first too, so we're good.

  write_registers(RX_ADDR_P0, reinterpret_cast<uint8_t*>(&value), addr_width);
  write_registers(TX_ADDR, reinterpret_cast<uint8_t*>(&value), addr_width);


  //const uint8_t max_payload_size = 32;
  //write_register(RX_PW_P0,rf24_min(payload_size,max_payload_size));
  write_register(RX_PW_P0,payload_size);
}

/****************************************************************************/
void openWritingPipe(const uint8_t *address)
{
  // Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
  // expects it LSB first too, so we're good.

  write_registers(RX_ADDR_P0,address, addr_width);
  write_registers(TX_ADDR, address, addr_width);

  //const uint8_t max_payload_size = 32;
  //write_register(RX_PW_P0,rf24_min(payload_size,max_payload_size));
  write_register(RX_PW_P0,payload_size);
}

/****************************************************************************/
static const uint8_t child_pipe[]  =
{
  RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5
};
static const uint8_t child_payload_size[] =
{
  RX_PW_P0, RX_PW_P1, RX_PW_P2, RX_PW_P3, RX_PW_P4, RX_PW_P5
};


void openReadingPipe(uint8_t child, uint64_t address)
{
  // If this is pipe 0, cache the address.  This is needed because
  // openWritingPipe() will overwrite the pipe 0 address, so
  // startListening() will have to restore it.
  if (child == 0){
    memcpy(pipe0_reading_address,&address,addr_width);
  }

  if (child <= 6)
  {
    // For pipes 2-5, only write the LSB
    if ( child < 2 )
      write_register(child_pipe[child], reinterpret_cast<const uint8_t*>(&address), addr_width);
    else
      write_register(child_pipe[child], reinterpret_cast<const uint8_t*>(&address), 1);

    write_register(child_payload_size[child],payload_size);

    // Note it would be more efficient to set all of the bits for all open
    // pipes at once.  However, I thought it would make the calling code
    // more simple to do it this way.
    write_register(EN_RXADDR,read_register(EN_RXADDR) | _BV(child_pipe_enable[child]));
  }
}

/****************************************************************************/
void setAddressWidth(uint8_t a_width){

        if(a_width -= 2){
                write_register(SETUP_AW,a_width%4);
                addr_width = (a_width%4) + 2;
        }else{
        write_register(SETUP_AW,0);
        addr_width = 2;
    }

}

/****************************************************************************/

void openReadingPipe(uint8_t child, const uint8_t *address)
{
  // If this is pipe 0, cache the address.  This is needed because
  // openWritingPipe() will overwrite the pipe 0 address, so
  // startListening() will have to restore it.
  if (child == 0){
    memcpy(pipe0_reading_address,address,addr_width);
  }
  if (child <= 6)
  {
    // For pipes 2-5, only write the LSB
    if ( child < 2 ){
      write_register(child_pipe[child], address, addr_width);
    }else{
      write_register(child_pipe[child], address, 1);
        }
    write_register(child_payload_size[child],payload_size);

    // Note it would be more efficient to set all of the bits for all open
    // pipes at once.  However, I thought it would make the calling code
    // more simple to do it this way.
    write_register(EN_RXADDR,read_register(EN_RXADDR) | _BV(child_pipe_enable[child]));

  }
}

/****************************************************************************/

void closeReadingPipe( uint8_t pipe )
{
  write_register(EN_RXADDR,read_register(EN_RXADDR) & ~_BV(child_pipe_enable[pipe]));
}

/****************************************************************************/

void toggle_features(void)
{
  csn(LOW);

  spi_transfer( ACTIVATE );
  spi_transfer( 0x73 );

  csn(HIGH);
}

/****************************************************************************/

void enableDynamicPayloads(void)
{
  // Enable dynamic payload throughout the system

    //toggle_features();
    write_register(FEATURE,read_register(FEATURE) | _BV(EN_DPL) );


  // Enable dynamic payload on all pipes
  //
  // Not sure the use case of only having dynamic payload on certain
  // pipes, so the library does not support it.
  write_register(DYNPD,read_register(DYNPD) | _BV(DPL_P5) | _BV(DPL_P4) | _BV(DPL_P3) | _BV(DPL_P2) | _BV(DPL_P1) | _BV(DPL_P0));

  dynamic_payloads_enabled = true;
}

/****************************************************************************/
void disableDynamicPayloads(void)
{
  // Disables dynamic payload throughout the system.  Also disables Ack Payloads

  //toggle_features();
  write_register(FEATURE, 0);


  // Disable dynamic payload on all pipes
  //
  // Not sure the use case of only having dynamic payload on certain
  // pipes, so the library does not support it.
  write_register(DYNPD, 0);

  dynamic_payloads_enabled = false;
}

/****************************************************************************/

void enableAckPayload(void)
{
  //
  // enable ack payload and dynamic payload features
  //

    //toggle_features();
    write_register(FEATURE,read_register(FEATURE) | _BV(EN_ACK_PAY) | _BV(EN_DPL) );
  //
  // Enable dynamic payload on pipes 0 & 1
  //

  write_register(DYNPD,read_register(DYNPD) | _BV(DPL_P1) | _BV(DPL_P0));
  dynamic_payloads_enabled = true;
}

/****************************************************************************/

void enableDynamicAck(void){
  //
  // enable dynamic ack features
  //
    //toggle_features();
    write_register(FEATURE,read_register(FEATURE) | _BV(EN_DYN_ACK) );

}

/****************************************************************************/

void writeAckPayload(uint8_t pipe, const void* buf, uint8_t len)
{
  const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);

  uint8_t data_len = rf24_min(len,32);

  csn(LOW);

  spi_transfer(W_ACK_PAYLOAD | ( pipe & 0x07 ) );

  while ( data_len-- )
    spi_transfer(*current++);

  csn(HIGH);
}

/****************************************************************************/

uint8_t isAckPayloadAvailable(void)
{
  return ! (read_register(FIFO_STATUS) & _BV(RX_EMPTY));
}

/****************************************************************************/

uint8_t isPVariant(void)
{
  return p_variant ;
}

/****************************************************************************/

void setAutoAck(uint8_t enable)
{
  if ( enable )
    write_register(EN_AA, 0x3F);
  else
    write_register(EN_AA, 0);
}

/****************************************************************************/

void setAutoAck( uint8_t pipe, uint8_t enable )
{
  if ( pipe <= 6 )
  {
    uint8_t en_aa = read_register( EN_AA ) ;
    if( enable )
    {
      en_aa |= _BV(pipe) ;
    }
    else
    {
      en_aa &= ~_BV(pipe) ;
    }
    write_register( EN_AA, en_aa ) ;
  }
}

/****************************************************************************/

uint8_t testCarrier(void)
{
  return ( read_register(CD) & 1 );
}

/****************************************************************************/

uint8_t testRPD(void)
{
  return ( read_register(RPD) & 1 ) ;
}

/****************************************************************************/

void setPALevel(uint8_t level)
{

  uint8_t setup = read_register(RF_SETUP) & 0xF8;

  if(level > 3){                                                // If invalid level, go to max PA
          level = (RF24_PA_MAX << 1) + 1;               // +1 to support the SI24R1 chip extra bit
  }else{
          level = (level << 1) + 1;                     // Else set level as requested
  }


  write_register( RF_SETUP, setup |= level ) ;  // Write it to the chip
}

/****************************************************************************/

uint8_t getPALevel(void)
{

  return (read_register(RF_SETUP) & (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH))) >> 1 ;
}

/****************************************************************************/

uint8_t setDataRate(rf24_datarate_e speed)
{
  uint8_t result = false;
  uint8_t setup = read_register(RF_SETUP) ;

  // HIGH and LOW '00' is 1Mbs - our default
  setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH)) ;

    txDelay=85;

  if( speed == RF24_250KBPS )
  {
    // Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
    // Making it '10'.
    setup |= _BV( RF_DR_LOW ) ;
    txDelay=155;
  }
  else
  {
    // Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
    // Making it '01'
    if ( speed == RF24_2MBPS )
    {
      setup |= _BV(RF_DR_HIGH);
      txDelay=65;
    }
  }
  write_register(RF_SETUP,setup);

  // Verify our result
  if ( read_register(RF_SETUP) == setup )
  {
    result = true;
  }
  return result;
}

/****************************************************************************/

rf24_datarate_e getDataRate( void )
{
  rf24_datarate_e result ;
  uint8_t dr = read_register(RF_SETUP) & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));

  // switch uses RAM (evil!)
  // Order matters in our case below
  if ( dr == _BV(RF_DR_LOW) )
  {
    // '10' = 250KBPS
    result = RF24_250KBPS ;
  }
  else if ( dr == _BV(RF_DR_HIGH) )
  {
    // '01' = 2MBPS
    result = RF24_2MBPS ;
  }
  else
  {
    // '00' = 1MBPS
    result = RF24_1MBPS ;
  }
  return result ;
}

/****************************************************************************/

void setCRCLength(rf24_crclength_e length)
{
  uint8_t config = read_register(NRF_CONFIG) & ~( _BV(CRCO) | _BV(EN_CRC)) ;

  // switch uses RAM (evil!)
  if ( length == RF24_CRC_DISABLED )
  {
    // Do nothing, we turned it off above.
  }
  else if ( length == RF24_CRC_8 )
  {
    config |= _BV(EN_CRC);
  }
  else
  {
    config |= _BV(EN_CRC);
    config |= _BV( CRCO );
  }
  write_register( NRF_CONFIG, config ) ;
}

/****************************************************************************/

rf24_crclength_e getCRCLength(void)
{
  rf24_crclength_e result = RF24_CRC_DISABLED;

  uint8_t config = read_register(NRF_CONFIG) & ( _BV(CRCO) | _BV(EN_CRC)) ;
  uint8_t AA = read_register(EN_AA);

  if ( config & _BV(EN_CRC ) || AA)
  {
    if ( config & _BV(CRCO) )
      result = RF24_CRC_16;
    else
      result = RF24_CRC_8;
  }

  return result;
}

/****************************************************************************/

void disableCRC( void )
{
  uint8_t disable = read_register(NRF_CONFIG) & ~_BV(EN_CRC) ;
  write_register( NRF_CONFIG, disable ) ;
}

/****************************************************************************/
void setRetries(uint8_t delay, uint8_t count)
{
 write_register(SETUP_RETR,(delay&0xf)<<ARD | (count&0xf)<<ARC);
}

#endif
