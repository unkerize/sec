
#define TIM_COUNT 11

const TIM_DRV_T tim_list[TIM_COUNT] = {

// 0
{ TIM1, &(RCC->APB2ENR), RCC_APB2ENR_TIM1EN, F_APB2_TIM},

// 1
#if defined(RCC_APB1ENR_TIM2EN)
{ TIM2, &(RCC->APB1ENR), RCC_APB1ENR_TIM2EN, F_APB1_TIM },
#else
{ 0, 0, 0, 0},
#endif

// 2
#if defined(RCC_APB1ENR_TIM3EN)
{ TIM3, &(RCC->APB1ENR), RCC_APB1ENR_TIM3EN, F_APB1_TIM },
#else
{ 0, 0, 0, 0},
#endif

// 3
#if defined(RCC_APB1ENR_TIM4EN)
{ TIM4, &(RCC->APB1ENR), RCC_APB1ENR_TIM4EN, F_APB1_TIM },
#else
{ 0, 0, 0, 0},
#endif

// 4
#if defined(RCC_APB1ENR_TIM6EN)
{ TIM6, &(RCC->APB1ENR), RCC_APB1ENR_TIM6EN,F_APB1_TIM },
#else
{ 0, 0, 0, 0},
#endif

// 5
#if defined(RCC_APB1ENR_TIM7EN)
{ TIM7, &(RCC->APB1ENR), RCC_APB1ENR_TIM7EN,F_APB1_TIM },
#else
{ 0, 0, 0, 0},
#endif

// 6
#if defined(RCC_APB2ENR_TIM8EN)
{ TIM8, &(RCC->APB2ENR), RCC_APB2ENR_TIM8EN, F_APB2_TIM},
#else
{ 0, 0, 0, 0},
#endif


// 7
#if defined(RCC_APB2ENR_TIM15EN)
{ TIM15, &(RCC->APB2ENR), RCC_APB2ENR_TIM15EN, F_APB2_TIM },
#else
{ 0, 0, 0, 0},
#endif

// 8
#if defined(RCC_APB2ENR_TIM16EN)
{ TIM16, &(RCC->APB2ENR), RCC_APB2ENR_TIM16EN, F_APB2_TIM },
#else
{ 0, 0, 0, 0},
#endif

// 9
#if defined(RCC_APB2ENR_TIM17EN)
{ TIM17, &(RCC->APB2ENR), RCC_APB2ENR_TIM17EN, F_APB2_TIM },
#else
{ 0, 0, 0, 0},
#endif


// 10
#if defined(RCC_APB2ENR_TIM20EN)
{ TIM20, &(RCC->APB2ENR), RCC_APB2ENR_TIM20EN, F_APB2_TIM}
#else
{ 0, 0, 0, 0}
#endif
};
