#ifndef __CHEBY__TOFPCB__H__
#define __CHEBY__TOFPCB__H__
#define TOFPCB_SIZE 56

/* None */
#define TOFPCB_ID 0x0UL
#define TOFPCB_ID_ID_MASK 0xffffffffUL
#define TOFPCB_ID_ID_SHIFT 0

/* None */
#define TOFPCB_PWM_COMP_LEVEL_1 0x4UL
#define TOFPCB_PWM_COMP_LEVEL_1_PWM_COMP_LEVEL_1_MASK 0xffffffffUL
#define TOFPCB_PWM_COMP_LEVEL_1_PWM_COMP_LEVEL_1_SHIFT 0

/* None */
#define TOFPCB_PWM_COMP_LEVEL_2 0x8UL
#define TOFPCB_PWM_COMP_LEVEL_2_PWM_COMP_LEVEL_2_MASK 0xffffffffUL
#define TOFPCB_PWM_COMP_LEVEL_2_PWM_COMP_LEVEL_2_SHIFT 0

/* None */
#define TOFPCB_PWM_V_ADJ_APD 0xcUL
#define TOFPCB_PWM_V_ADJ_APD_PWM_V_ADJ_APD_MASK 0xffffffffUL
#define TOFPCB_PWM_V_ADJ_APD_PWM_V_ADJ_APD_SHIFT 0

/* None */
#define TOFPCB_V_5V_SENSE 0x10UL
#define TOFPCB_V_5V_SENSE_V_5V_SENSE_MASK 0xffffffffUL
#define TOFPCB_V_5V_SENSE_V_5V_SENSE_SHIFT 0

/* None */
#define TOFPCB_V_APD_SENSE 0x14UL
#define TOFPCB_V_APD_SENSE_V_APD_SENSE_MASK 0xffffffffUL
#define TOFPCB_V_APD_SENSE_V_APD_SENSE_SHIFT 0

/* None */
#define TOFPCB_V_APD_R_SENSE 0x18UL
#define TOFPCB_V_APD_R_SENSE_V_APD_R_SENSE_MASK 0xffffffffUL
#define TOFPCB_V_APD_R_SENSE_V_APD_R_SENSE_SHIFT 0

/* None */
#define TOFPCB_V_SIPM_SENSE 0x1cUL
#define TOFPCB_V_SIPM_SENSE_V_SIPM_SENSE_MASK 0xffffffffUL
#define TOFPCB_V_SIPM_SENSE_V_SIPM_SENSE_SHIFT 0

/* None */
#define TOFPCB_LOCKIN_1_FILT 0x20UL
#define TOFPCB_LOCKIN_1_FILT_LOCKIN_1_FILT_MASK 0xffffffffUL
#define TOFPCB_LOCKIN_1_FILT_LOCKIN_1_FILT_SHIFT 0

/* None */
#define TOFPCB_LOCKIN_2_FILT 0x24UL
#define TOFPCB_LOCKIN_2_FILT_LOCKIN_2_FILT_MASK 0xffffffffUL
#define TOFPCB_LOCKIN_2_FILT_LOCKIN_2_FILT_SHIFT 0

/* None */
#define TOFPCB_LOCKIN_1_PEAK 0x28UL
#define TOFPCB_LOCKIN_1_PEAK_LOCKIN_1_PEAK_MASK 0xffffffffUL
#define TOFPCB_LOCKIN_1_PEAK_LOCKIN_1_PEAK_SHIFT 0

/* None */
#define TOFPCB_TEMPERATURE 0x2cUL
#define TOFPCB_TEMPERATURE_TEMPERATURE_MASK 0xffffffffUL
#define TOFPCB_TEMPERATURE_TEMPERATURE_SHIFT 0

/* None */
#define TOFPCB_START_PULSES 0x30UL
#define TOFPCB_START_PULSES_START_PULSES_MASK 0xffffffffUL
#define TOFPCB_START_PULSES_START_PULSES_SHIFT 0

/* None */
#define TOFPCB_STOP_PULSES 0x34UL
#define TOFPCB_STOP_PULSES_STOP_PULSES_MASK 0xffffffffUL
#define TOFPCB_STOP_PULSES_STOP_PULSES_SHIFT 0

struct TofPCB {
  /* [0x0]: REG (rw) (no description) */
  uint32_t id;

  /* [0x4]: REG (rw) (no description) */
  uint32_t pwm_comp_level_1;

  /* [0x8]: REG (rw) (no description) */
  uint32_t pwm_comp_level_2;

  /* [0xc]: REG (rw) (no description) */
  uint32_t pwm_v_adj_apd;

  /* [0x10]: REG (rw) (no description) */
  uint32_t v_5v_sense;

  /* [0x14]: REG (rw) (no description) */
  uint32_t v_apd_sense;

  /* [0x18]: REG (rw) (no description) */
  uint32_t v_apd_r_sense;

  /* [0x1c]: REG (rw) (no description) */
  uint32_t v_sipm_sense;

  /* [0x20]: REG (rw) (no description) */
  uint32_t lockin_1_filt;

  /* [0x24]: REG (rw) (no description) */
  uint32_t lockin_2_filt;

  /* [0x28]: REG (rw) (no description) */
  uint32_t lockin_1_peak;

  /* [0x2c]: REG (rw) (no description) */
  uint32_t temperature;

  /* [0x30]: REG (rw) (no description) */
  uint32_t start_pulses;

  /* [0x34]: REG (rw) (no description) */
  uint32_t stop_pulses;
};

#endif /* __CHEBY__TOFPCB__H__ */
