#pragma once
#pragma pack(push, 1)

struct GCONF_t {
  constexpr static uint8_t address = 0x00;
  union {
    uint32_t sr : 18;
    struct {
      bool  i_scale_analog : 1, // 2130, 5130
            internal_rsense : 1, // 2130, 5130
            en_pwm_mode : 1,
            enc_commutation : 1, // 2130, 5130
            shaft : 1,
            diag0_error : 1,
            diag0_otpw : 1,
            diag0_stall : 1,
            diag1_stall : 1,
            diag1_index : 1,
            diag1_onstate : 1,
            diag1_steps_skipped : 1,
            diag0_int_pushpull : 1,
            diag1_pushpull : 1,
            small_hysteresis : 1,
            stop_enable : 1,
            direct_mode : 1;
    };
    struct { // TMC5160
      bool recalibrate : 1,
           faststandstill : 1,
                          : 1,
           multistep_filt : 1,
                    : 3,
           diag0_step : 1,
           diag1_dir : 1,
                 : 4,
           diag1_poscomp_pushpull : 1;
    };
  };
};

struct IHOLD_IRUN_t {
  constexpr static uint8_t address = 0x10;
  union {
    uint32_t sr : 20;
    struct {
      uint8_t ihold : 5,
                    : 3,
              irun : 5,
                   : 3,
              iholddelay : 4;
    };
  };
};

struct GSTAT_t {
  constexpr static uint8_t address = 0x01;
  union {
    uint8_t sr : 3;
    struct {
      bool  reset : 1,
            drv_err : 1,
            uv_cp : 1;
    };
  };
};

struct IOIN_t {
  constexpr static uint8_t address = 0x04;
  union {
    uint32_t sr;
    struct {
      bool  step : 1,
            dir : 1,
            dcen_cfg4 : 1,
            dcin_cfg5 : 1,
            drv_enn_cfg6 : 1,
            dco : 1,
            : 2;
      uint16_t : 16;
      uint8_t version : 8;
    };
  };
};

struct TPOWERDOWN_t {
  constexpr static uint8_t address = 0x11;
  uint8_t sr : 8;
};

struct TPWMTHRS_t {
  constexpr static uint8_t address = 0x13;
  uint32_t sr : 20;
};

struct TCOOLTHRS_t {
  constexpr static uint8_t address = 0x14;
  uint32_t sr : 20;
};

struct THIGH_t {
  constexpr static uint8_t address = 0x15;
  uint32_t sr : 20;
};

struct XDIRECT_t {
  constexpr static uint8_t address = 0x2D;
  union {
    uint32_t sr : 25;
    struct {
      int16_t coil_A : 9;
      int8_t         : 7;
      int16_t coil_B : 9;
    };
  };
};

struct VDCMIN_t {
  constexpr static uint8_t address = 0x33;
  uint32_t sr : 23;
};

struct CHOPCONF_t {
  constexpr static uint8_t address = 0x6C;
  union {
    uint32_t sr : 32;
    struct {
      uint8_t toff : 4,
              hstrt : 3,
              hend : 4,
                   : 1;
      bool    disfdcc : 1,
              rndtf : 1,
              chm : 1;
      uint8_t tbl : 2;
      bool    vsense : 1,
              vhighfs : 1,
              vhighchm : 1;
      uint8_t sync : 4, // 2130, 5130
              mres : 4;
      bool    intpol : 1,
              dedge : 1,
              diss2g : 1;
    };
    struct { // TMC5160
      uint32_t     : 20;
      uint8_t tpfd : 4; // 5160
      uint16_t     : 7;
      bool diss2vs : 1; // TMC5160 only
    };
  };
};

struct COOLCONF_t {
  constexpr static uint8_t address = 0x6D;
  union {
    uint32_t sr : 25;
    struct {
      uint8_t semin : 4,
                    : 1,
              seup : 2,
                    : 1,
              semax : 4,
                    : 1,
              sedn : 2;
      bool    seimin : 1;
      int8_t  sgt : 7,
                  : 1;
      bool    sfilt : 1;
    };
  };
};

struct DCCTRL_t {
	constexpr static uint8_t address = 0x6E;
	union {
		uint32_t sr : 24;
		struct {
			uint16_t dc_time : 10,
				: 6;
			uint8_t dc_sg : 8;
		};
	};
};

namespace TMC2130_n {
  struct DRV_STATUS_t {
    constexpr static uint8_t address = 0x6F;
    union {
      uint32_t sr;
      struct {
        uint16_t sg_result : 10;
        uint8_t            : 5;
        bool fsactive : 1;
        uint8_t cs_actual : 5,
                          : 3;
        bool  stallGuard : 1,
              ot : 1,
              otpw : 1,
              s2ga : 1,
              s2gb : 1,
              ola : 1,
              olb : 1,
              stst : 1;
      };
    };
  };
}

struct PWMCONF_t {
  constexpr static uint8_t address = 0x70;
  union {
    uint32_t sr : 22;
    struct {
      uint8_t pwm_ampl : 8,
              pwm_grad : 8,
              pwm_freq : 2;
      bool pwm_autoscale : 1,
           pwm_symmetric : 1;
      uint8_t freewheel : 2;
    };
  };
};

struct ENCM_CTRL_t {
  constexpr static uint8_t address = 0x72;
  union {
    uint8_t sr : 2;
    struct {
      bool  inv : 1,
            maxspeed : 1;
    };
  };
};

struct SHORT_CONF_t {
  constexpr static uint8_t address = 0x09;
  union {
    uint32_t sr : 19;
    struct {
      uint8_t s2vs_level  : 4,
                          : 4,
              s2g_level   : 4,
                          : 4,
              shortfilter : 2;
      bool shortdelay : 1;
    };
  };
};

struct DRV_CONF_t {
  constexpr static uint8_t address = 0x0A;
  union {
    uint32_t sr : 22;
    struct {
      uint8_t bbmtime : 5,
                      : 3,
              bbmclks : 4,
                      : 4,
              otselect : 2,
              drvstrength : 2,
              filt_isense : 2;
    };
  };
};

struct GLOBAL_SCALER_t {
  constexpr static uint8_t address = 0x0B;
  uint8_t sr;
};

struct OFFSET_READ_t {
  constexpr static uint8_t address = 0x0C;
};

struct ENC_DEVIATION_t {
  constexpr static uint8_t address = 0x3D;
  uint32_t sr : 20;
};

struct PWM_AUTO_t {
  constexpr static uint8_t address = 0x72;
  union {
    uint32_t sr : 24;
    struct {
      uint8_t pwm_ofs_auto : 8,
                           : 8,
              pwm_grad_auto : 8;
    };
  };
};

struct SLAVECONF_t {
  constexpr static uint8_t address = 0x03;
  union {
    uint16_t sr : 12;
    struct {
      uint8_t slaveaddr : 8;
      uint8_t senddelay : 4;
    };
  };
};

namespace TMC5130_n {
  struct IOIN_t {
    constexpr static uint8_t address = 0x04;
    union {
      uint32_t sr;
      struct {
        bool  refl_step : 1,
              refr_dir : 1,
              encb_dcen_cfg4 : 1,
              enca_dcin_cfg5 : 1,
              drv_enn_cfg6 : 1,
              enc_n_dco : 1,
              sd_mode : 1,
              swcomp_in : 1;
        uint16_t : 16;
        uint8_t version : 8;
      };
    };
  };
}

struct OUTPUT_t {
  constexpr static uint8_t address = 0x04;
  bool sr : 1;
};

struct X_COMPARE_t {
  constexpr static uint8_t address = 0x05;
  uint32_t sr;
};

struct RAMPMODE_t {
  constexpr static uint8_t address = 0x20;
  uint8_t sr : 2;
};

struct XACTUAL_t {
  constexpr static uint8_t address = 0x21;
  uint32_t sr;
};

struct VSTART_t {
  constexpr static uint8_t address = 0x23;
  uint32_t sr : 18;
};

struct A1_t {
  constexpr static uint8_t address = 0x24;
  uint16_t sr : 16;
};

struct V1_t {
  constexpr static uint8_t address = 0x25;
  uint32_t sr : 20;
};

struct AMAX_t {
  constexpr static uint8_t address = 0x26;
  uint16_t sr : 16;
};

struct VMAX_t {
  constexpr static uint8_t address = 0x27;
  uint32_t sr : 23;
};

struct DMAX_t {
  constexpr static uint8_t address = 0x28;
  uint16_t sr : 16;
};

struct D1_t {
  constexpr static uint8_t address = 0x2A;
  uint16_t sr : 16;
};

struct VSTOP_t {
  constexpr static uint8_t address = 0x2B;
  uint32_t sr : 18;
};

struct TZEROWAIT_t {
  constexpr static uint8_t address = 0x2C;
  uint16_t sr : 16;
};

struct SW_MODE_t {
  constexpr static uint8_t address = 0x34;
  union {
    uint16_t sr : 12;
    struct {
      bool  stop_l_enable : 1,
            stop_r_enable : 1,
            pol_stop_l : 1,
            pol_stop_r : 1,
            swap_lr : 1,
            latch_l_active : 1,
            latch_l_inactive : 1,
            latch_r_active : 1,
            latch_r_inactive : 1,
            en_latch_encoder : 1,
            sg_stop : 1,
            en_softstop : 1;
    };
  };
};

struct RAMP_STAT_t {
  constexpr static uint8_t address = 0x35;
  union {
    uint16_t sr : 14;
    struct {
      bool  status_stop_l : 1,
            status_stop_r : 1,
            status_latch_l : 1,
            status_latch_r : 1,
            event_stop_l : 1,
            event_stop_r : 1,
            event_stop_sg : 1,
            event_pos_reached : 1,
            velocity_reached : 1,
            position_reached : 1,
            vzero : 1,
            t_zerowait_active : 1,
            second_move : 1,
            status_sg : 1;
    };
  };
};

struct ENCMODE_t {
  constexpr static uint8_t address = 0x38;
  union {
    uint16_t sr : 11;
    struct {
      bool  pol_a : 1,
            pol_b : 1,
            pol_n : 1,
            ignore_ab : 1,
            clr_cont : 1,
            clr_once : 1,
            pos_edge : 1,
            neg_edge : 1,
            clr_enc_x : 1,
            latch_x_act : 1,
            enc_sel_decimal : 1;
    };
  };
};

struct ENC_CONST_t {
  constexpr static uint8_t address = 0x3A;
  uint32_t sr;
};

struct MSLUT0_t {
  constexpr static uint8_t address = 0x60;
  uint32_t sr;
};

struct MSLUT1_t {
  constexpr static uint8_t address = 0x61;
  uint32_t sr;
};

struct MSLUT2_t {
  constexpr static uint8_t address = 0x62;
  uint32_t sr;
};

struct MSLUT3_t {
  constexpr static uint8_t address = 0x63;
  uint32_t sr;
};

struct MSLUT4_t {
  constexpr static uint8_t address = 0x64;
  uint32_t sr;
};

struct MSLUT5_t {
  constexpr static uint8_t address = 0x65;
  uint32_t sr;
};

struct MSLUT6_t {
  constexpr static uint8_t address = 0x66;
  uint32_t sr;
};

struct MSLUT7_t {
  constexpr static uint8_t address = 0x67;
  uint32_t sr;
};

struct MSLUTSEL_t {
  constexpr static uint8_t address = 0x68;
  union {
    uint32_t sr;
    struct {
      uint8_t w0 : 2,
              w1 : 2,
              w2 : 2,
              w3 : 2,
              x1 : 8,
              x2 : 8,
              x3 : 8;
    };
  };
};

struct MSLUTSTART_t {
  constexpr static uint8_t address = 0x69;
  union {
    uint32_t sr : 24;
    struct {
      uint8_t start_sin : 8,
                        : 8,
              start_sin90 : 8;
    };
  };
};

#pragma pack(pop)
