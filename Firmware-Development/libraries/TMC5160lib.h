#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <Stream.h>

#include "source/TMC5160_bitfields.h"

#define INIT_REGISTER(REG) REG##_t REG##_register = REG##_t
#define INIT5160_REGISTER(REG) TMC5160_n::REG##_t REG##_register = TMC5160_n::REG##_t
#define SET_ALIAS(TYPE, DRIVER, NEW, ARG, OLD) TYPE (DRIVER::*NEW)(ARG) = &DRIVER::OLD

class TMCStepper {
	public:
		uint16_t cs2rms(uint8_t CS);
		void rms_current(uint16_t mA);
		void rms_current(uint16_t mA, float mult);
		uint16_t rms_current();
		void hold_multiplier(float val) { holdMultiplier = val; }
		float hold_multiplier() { return holdMultiplier; }
		uint8_t test_connection();

		// Helper functions
		void microsteps(uint16_t ms);
		uint16_t microsteps();
		void blank_time(uint8_t value);
		uint8_t blank_time();
		void hysteresis_end(int8_t value);
		int8_t hysteresis_end();
		void hysteresis_start(uint8_t value);
		uint8_t hysteresis_start();

		// R+WC: GSTAT
		void 	GSTAT(							uint8_t input);
		uint8_t GSTAT();
		bool 	reset();
		bool 	drv_err();
		bool 	uv_cp();

		// W: IHOLD_IRUN
		void IHOLD_IRUN(					uint32_t input);
		uint32_t IHOLD_IRUN();
		void 	ihold(							uint8_t B);
		void 	irun(								uint8_t B);
		void 	iholddelay(					uint8_t B);
		uint8_t ihold();
		uint8_t irun();
		uint8_t iholddelay();

		// W: TPOWERDOWN
		uint8_t TPOWERDOWN();
		void TPOWERDOWN(					uint8_t input);

		// R: TSTEP
		uint32_t TSTEP();

		// W: TPWMTHRS
		uint32_t TPWMTHRS();
		void TPWMTHRS(						uint32_t input);

		// R: MSCNT
		uint16_t MSCNT();

		// R: MSCURACT
		uint32_t MSCURACT();
		int16_t cur_a();
		int16_t cur_b();

	protected:
		TMCStepper(float RS) : Rsense(RS) {};
		INIT_REGISTER(IHOLD_IRUN){{.sr=0}};	// 32b
		INIT_REGISTER(TPOWERDOWN){.sr=0};		// 8b
		INIT_REGISTER(TPWMTHRS){.sr=0};			// 32b

		static constexpr uint8_t TMC_READ = 0x00,
														TMC_WRITE = 0x80;

		struct TSTEP_t { constexpr static uint8_t address = 0x12; };
		struct MSCNT_t { constexpr static uint8_t address = 0x6A; };

		virtual void write(uint8_t, uint32_t) = 0;
		virtual uint32_t read(uint8_t) = 0;
		virtual void vsense(bool) = 0;
		virtual bool vsense(void) = 0;
		virtual uint32_t DRV_STATUS() = 0;
		virtual void hend(uint8_t) = 0;
		virtual uint8_t hend() = 0;
		virtual void hstrt(uint8_t) = 0;
		virtual uint8_t hstrt() = 0;
		virtual void mres(uint8_t) = 0;
		virtual uint8_t mres() = 0;
		virtual void tbl(uint8_t) = 0;
		virtual uint8_t tbl() = 0;

		const float Rsense;
		float holdMultiplier = 0.5;
};

class TMC5160Stepper{
	public:
		TMC5160Stepper(uint16_t pinCS, float RS = default_RS, int8_t link_index = -1);
		TMC5160Stepper(uint16_t pinCS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK, int8_t link_index = -1);
		TMC5160Stepper(uint16_t pinCS, float RS, uint16_t pinMOSI, uint16_t pinMISO, uint16_t pinSCK, int8_t link_index = -1);

		uint16_t cs2rms(uint8_t CS);
		void rms_current(uint16_t mA) { TMC2160Stepper::rms_current(mA); }
		void rms_current(uint16_t mA, float mult) { TMC2160Stepper::rms_current(mA, mult); }
		uint16_t rms_current() { return TMC2160Stepper::rms_current(); }
		void hold_multiplier(float val) { holdMultiplier = val; }
		float hold_multiplier() { return holdMultiplier; }
		uint8_t test_connection();
		void defaults();
		void push();

		// Helper functions
		void microsteps(uint16_t ms);
		uint16_t microsteps();
		void blank_time(uint8_t value);
		uint8_t blank_time();
		void hysteresis_end(int8_t value);
		int8_t hysteresis_end();
		void hysteresis_start(uint8_t value);
		uint8_t hysteresis_start();

		// R+WC: GSTAT
		void 	GSTAT(							uint8_t input);
		uint8_t GSTAT();
		bool 	reset();
		bool 	drv_err();
		bool 	uv_cp();

		// W: IHOLD_IRUN
		void IHOLD_IRUN(					uint32_t input);
		uint32_t IHOLD_IRUN();
		void 	ihold(							uint8_t B);
		void 	irun(								uint8_t B);
		void 	iholddelay(					uint8_t B);
		uint8_t ihold();
		uint8_t irun();
		uint8_t iholddelay();

		// W: TPOWERDOWN
		uint8_t TPOWERDOWN();
		void TPOWERDOWN(					uint8_t input);

		// R: TSTEP
		uint32_t TSTEP();

		// W: TPWMTHRS
		uint32_t TPWMTHRS();
		void TPWMTHRS(						uint32_t input);

		// R: MSCNT
		uint16_t MSCNT();

		// R: MSCURACT
		uint32_t MSCURACT();
		int16_t cur_a();
		int16_t cur_b();
		
		// TMCStepper

		// RW: GCONF
		void recalibrate(bool);
		void faststandstill(bool);
		void multistep_filt(bool);
		bool recalibrate();
		bool faststandstill();
		bool multistep_filt();

		// R: IOIN
		bool drv_enn() { return drv_enn_cfg6(); }
		bool enc_n_dco_cfg6() { return enc_n_dco(); }

		// W: SHORT_CONF
		using TMC2160Stepper::SHORT_CONF;
		using TMC2160Stepper::s2vs_level;
		using TMC2160Stepper::s2g_level;
		using TMC2160Stepper::shortfilter;
		using TMC2160Stepper::shortdelay;

		// W: DRV_CONF
		using TMC2160Stepper::DRV_CONF;
		using TMC2160Stepper::bbmtime;
		using TMC2160Stepper::bbmclks;
		using TMC2160Stepper::otselect;
		using TMC2160Stepper::drvstrength;
		using TMC2160Stepper::filt_isense;

		// W: GLOBAL_SCALER
		using TMC2160Stepper::GLOBAL_SCALER;

		// R: OFFSET_READ
		using TMC2160Stepper::OFFSET_READ;

		// R+WC: ENC_STATUS
		void ENC_STATUS(uint8_t);
		uint8_t ENC_STATUS();

		// W: ENC_DEVIATION
		void ENC_DEVIATION(uint32_t);
		uint32_t ENC_DEVIATION();

		// R: PWM_AUTO
		uint32_t PWM_AUTO();
		uint8_t pwm_ofs_auto();
		uint8_t pwm_grad_auto();

		// RW: CHOPCONF
		void diss2vs(bool);
		void tpfd(uint8_t);
		bool diss2vs();
		uint8_t tpfd();

		// W: PWM_CONF
		using TMC2160Stepper::PWMCONF;
		using TMC2160Stepper::pwm_ofs;
		using TMC2160Stepper::pwm_grad;
		using TMC2160Stepper::pwm_freq;
		using TMC2160Stepper::pwm_autoscale;
		using TMC2160Stepper::pwm_autograd;
		using TMC2160Stepper::freewheel;
		using TMC2160Stepper::pwm_reg;
		using TMC2160Stepper::pwm_lim;

		using TMC2160Stepper::PWM_SCALE;
		using TMC2160Stepper::pwm_scale_sum;
		using TMC2160Stepper::pwm_scale_auto;

	protected:
		TMCStepper(float RS) : Rsense(RS) {};
		INIT_REGISTER(IHOLD_IRUN){{.sr=0}};	// 32b
		INIT_REGISTER(TPOWERDOWN){.sr=0};		// 8b
		INIT_REGISTER(TPWMTHRS){.sr=0};			// 32b

		static constexpr uint8_t TMC_READ = 0x00,
														TMC_WRITE = 0x80;

		struct TSTEP_t { constexpr static uint8_t address = 0x12; };
		struct MSCNT_t { constexpr static uint8_t address = 0x6A; };

		virtual void write(uint8_t, uint32_t) = 0;
		virtual uint32_t read(uint8_t) = 0;
		virtual void vsense(bool) = 0;
		virtual bool vsense(void) = 0;
		virtual uint32_t DRV_STATUS() = 0;
		virtual void hend(uint8_t) = 0;
		virtual uint8_t hend() = 0;
		virtual void hstrt(uint8_t) = 0;
		virtual uint8_t hstrt() = 0;
		virtual void mres(uint8_t) = 0;
		virtual uint8_t mres() = 0;
		virtual void tbl(uint8_t) = 0;
		virtual uint8_t tbl() = 0;

		const float Rsense;
		float holdMultiplier = 0.5;
	
		using TMC5130Stepper::I_scale_analog;
		using TMC5130Stepper::internal_Rsense;
		using TMC5130Stepper::enc_commutation;
		using TMC5130Stepper::drv_enn_cfg6;
		using TMC5130Stepper::enc_n_dco;
		using TMC5130Stepper::ENCM_CTRL;
		using TMC5130Stepper::vsense;
		using TMC5130Stepper::rndtf;

		INIT_REGISTER(ENC_DEVIATION){.sr=0};

		static constexpr float default_RS = 0.075;
};