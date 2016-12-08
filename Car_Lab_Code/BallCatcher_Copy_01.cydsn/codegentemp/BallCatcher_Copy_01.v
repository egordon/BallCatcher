// ======================================================================
// BallCatcher_Copy_01.v generated from TopDesign.cysch
// 05/02/2016 at 22:48
// This file is auto generated. ANY EDITS YOU MAKE MAY BE LOST WHEN THIS FILE IS REGENERATED!!!
// ======================================================================

/* -- WARNING: The following section of defines are deprecated and will be removed in a future release -- */
`define CYDEV_CHIP_DIE_LEOPARD 1
`define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3
`define CYDEV_CHIP_REV_LEOPARD_ES3 3
`define CYDEV_CHIP_REV_LEOPARD_ES2 1
`define CYDEV_CHIP_REV_LEOPARD_ES1 0
`define CYDEV_CHIP_DIE_PANTHER 2
`define CYDEV_CHIP_REV_PANTHER_PRODUCTION 1
`define CYDEV_CHIP_REV_PANTHER_ES1 1
`define CYDEV_CHIP_REV_PANTHER_ES0 0
`define CYDEV_CHIP_DIE_EXPECT 1
`define CYDEV_CHIP_REV_EXPECT 3
`define CYDEV_CHIP_DIE_ACTUAL 1
/* -- WARNING: The previous section of defines are deprecated and will be removed in a future release -- */
`define CYDEV_CHIP_FAMILY_UNKNOWN 0
`define CYDEV_CHIP_MEMBER_UNKNOWN 0
`define CYDEV_CHIP_FAMILY_PSOC3 1
`define CYDEV_CHIP_MEMBER_3A 1
`define CYDEV_CHIP_REVISION_3A_PRODUCTION 3
`define CYDEV_CHIP_REVISION_3A_ES3 3
`define CYDEV_CHIP_REVISION_3A_ES2 1
`define CYDEV_CHIP_REVISION_3A_ES1 0
`define CYDEV_CHIP_FAMILY_PSOC5 2
`define CYDEV_CHIP_MEMBER_5A 2
`define CYDEV_CHIP_REVISION_5A_PRODUCTION 1
`define CYDEV_CHIP_REVISION_5A_ES1 1
`define CYDEV_CHIP_REVISION_5A_ES0 0
`define CYDEV_CHIP_FAMILY_USED 1
`define CYDEV_CHIP_MEMBER_USED 1
`define CYDEV_CHIP_REVISION_USED 3
// Component: cy_virtualmux_v1_0
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0\cy_virtualmux_v1_0.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_virtualmux_v1_0\cy_virtualmux_v1_0.v"
`endif

// Component: OneTerminal
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\OneTerminal"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\OneTerminal\OneTerminal.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\OneTerminal"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\OneTerminal\OneTerminal.v"
`endif

// Component: ZeroTerminal
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\ZeroTerminal"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\ZeroTerminal\ZeroTerminal.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\ZeroTerminal"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\ZeroTerminal\ZeroTerminal.v"
`endif

// PWM_v2_20(CaptureMode=0, Compare1_16=false, Compare1_8=false, Compare2_16=false, Compare2_8=false, CompareStatusEdgeSense=true, CompareType1=1, CompareType1Software=0, CompareType2=1, CompareType2Software=0, CompareValue1=500, CompareValue2=63, CONTROL3=1, ControlReg=false, CtlModeReplacementString=SyncCtl, CyGetRegReplacementString=CY_GET_REG16, CySetRegReplacementString=CY_SET_REG16, DeadBand=0, DeadBand2_4=0, DeadBand256=0, DeadBandUsed=0, DeadTime=1, DitherOffset=0, EnableMode=0, FF16=true, FF8=false, FixedFunction=true, FixedFunctionUsed=1, InterruptOnCMP1=false, InterruptOnCMP2=false, InterruptOnKill=false, InterruptOnTC=false, IntOnCMP1=0, IntOnCMP2=0, IntOnKill=0, IntOnTC=0, KillMode=1, KillModeMinTime=0, MinimumKillTime=1, OneCompare=true, Period=999, PWMMode=0, PWMModeCenterAligned=0, RegDefReplacementString=reg16, RegSizeReplacementString=uint16, Resolution=16, RstStatusReplacementString=sSTSReg_rstSts, RunMode=0, Status=false, TriggerMode=0, UDB16=false, UDB8=false, UseControl=true, UseInterrupt=false, UseStatus=false, VerilogSectionReplacementString=sP16, CY_COMPONENT_NAME=PWM_v2_20, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=FR_HBridge, CY_INSTANCE_SHORT_NAME=FR_HBridge, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=20, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=FR_HBridge, )
module PWM_v2_20_0 (
    pwm2,
    tc,
    clock,
    reset,
    pwm1,
    interrupt,
    capture,
    kill,
    enable,
    trigger,
    cmp_sel,
    pwm,
    ph1,
    ph2);
    output      pwm2;
    output      tc;
    input       clock;
    input       reset;
    output      pwm1;
    output      interrupt;
    input       capture;
    input       kill;
    input       enable;
    input       trigger;
    input       cmp_sel;
    output      pwm;
    output      ph1;
    output      ph2;

    parameter Resolution = 16;

          wire  Net_113;
          wire  Net_114;
          wire  Net_107;
          wire  Net_96;
          wire  Net_55;
          wire  Net_57;
          wire  Net_101;
          wire  Net_54;
          wire  Net_63;

    cy_psoc3_timer_v1_0 PWMHW (
        .timer_reset(reset),
        .capture(capture),
        .enable(Net_113),
        .kill(Net_107),
        .clock(clock),
        .tc(Net_63),
        .compare(Net_57),
        .interrupt(Net_54));

	// vmCompare (cy_virtualmux_v1_0)
	assign pwm = Net_57;

	// vmIRQ (cy_virtualmux_v1_0)
	assign interrupt = Net_54;

	// vmTC (cy_virtualmux_v1_0)
	assign tc = Net_63;

    OneTerminal OneTerminal_1 (
        .o(Net_113));

	// FFKillMux (cy_virtualmux_v1_0)
	assign Net_107 = kill;

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_114));



endmodule

// CharLCD_v1_60(ConversionRoutines=true, CUSTOM0=0,E,8,8,8,E,0, CUSTOM1=0,A,A,4,4,4,0, CUSTOM2=0,E,A,E,8,8,0, CUSTOM3=0,E,A,C,A,A,0, CUSTOM4=0,E,8,C,8,E,0, CUSTOM5=0,E,8,E,2,E,0, CUSTOM6=0,E,8,E,2,E,0, CUSTOM7=0,4,4,4,0,4,0, CustomCharacterSet=0, CY_COMPONENT_NAME=CharLCD_v1_60, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=LCD, CY_INSTANCE_SHORT_NAME=LCD, CY_MAJOR_VERSION=1, CY_MINOR_VERSION=60, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=LCD, )
module CharLCD_v1_60_1 ;



	wire [6:0] tmpOE__LCDPort_net;
	wire [6:0] tmpFB_6__LCDPort_net;
	wire [6:0] tmpIO_6__LCDPort_net;
	wire [0:0] tmpINTERRUPT_0__LCDPort_net;
	electrical [0:0] tmpSIOVREF__LCDPort_net;

	cy_psoc3_pins_v1_10
		#(.id("fdf5e9fe-6f44-4f01-a751-2ec325f7f38c/ed092b9b-d398-4703-be89-cebf998501f6"),
		  .drive_mode(21'b110_110_110_110_110_110_110),
		  .ibuf_enabled(7'b1_1_1_1_1_1_1),
		  .init_dr_st(7'b0_0_0_0_0_0_0),
		  .input_sync(7'b1_1_1_1_1_1_1),
		  .intr_mode(14'b00_00_00_00_00_00_00),
		  .io_voltage(", , , , , , "),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(7'b0_0_0_0_0_0_0),
		  .output_conn(7'b0_0_0_0_0_0_0),
		  .output_sync(7'b0_0_0_0_0_0_0),
		  .pin_aliases(",,,,,,"),
		  .pin_mode("OOOOOOO"),
		  .por_state(4),
		  .use_annotation(7'b0_0_0_0_0_0_0),
		  .sio_group_cnt(0),
		  .sio_hyst(7'b0_0_0_0_0_0_0),
		  .sio_ibuf(""),
		  .sio_info(14'b00_00_00_00_00_00_00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(7'b0_0_0_0_0_0_0),
		  .spanning(0),
		  .vtrip(14'b10_10_10_10_10_10_10),
		  .width(7))
		LCDPort
		 (.oe(tmpOE__LCDPort_net),
		  .y({7'b0}),
		  .fb({tmpFB_6__LCDPort_net[6:0]}),
		  .io({tmpIO_6__LCDPort_net[6:0]}),
		  .siovref(tmpSIOVREF__LCDPort_net),
		  .interrupt({tmpINTERRUPT_0__LCDPort_net[0:0]}));

	assign tmpOE__LCDPort_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{7'b1111111} : {7'b1111111};



endmodule

// PWM_v2_20(CaptureMode=0, Compare1_16=false, Compare1_8=false, Compare2_16=false, Compare2_8=false, CompareStatusEdgeSense=true, CompareType1=1, CompareType1Software=0, CompareType2=1, CompareType2Software=0, CompareValue1=500, CompareValue2=63, CONTROL3=1, ControlReg=false, CtlModeReplacementString=SyncCtl, CyGetRegReplacementString=CY_GET_REG16, CySetRegReplacementString=CY_SET_REG16, DeadBand=0, DeadBand2_4=0, DeadBand256=0, DeadBandUsed=0, DeadTime=1, DitherOffset=0, EnableMode=0, FF16=true, FF8=false, FixedFunction=true, FixedFunctionUsed=1, InterruptOnCMP1=false, InterruptOnCMP2=false, InterruptOnKill=false, InterruptOnTC=false, IntOnCMP1=0, IntOnCMP2=0, IntOnKill=0, IntOnTC=0, KillMode=1, KillModeMinTime=0, MinimumKillTime=1, OneCompare=true, Period=999, PWMMode=0, PWMModeCenterAligned=0, RegDefReplacementString=reg16, RegSizeReplacementString=uint16, Resolution=16, RstStatusReplacementString=sSTSReg_rstSts, RunMode=0, Status=false, TriggerMode=0, UDB16=false, UDB8=false, UseControl=true, UseInterrupt=false, UseStatus=false, VerilogSectionReplacementString=sP16, CY_COMPONENT_NAME=PWM_v2_20, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=BR_HBridge, CY_INSTANCE_SHORT_NAME=BR_HBridge, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=20, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=BR_HBridge, )
module PWM_v2_20_2 (
    pwm2,
    tc,
    clock,
    reset,
    pwm1,
    interrupt,
    capture,
    kill,
    enable,
    trigger,
    cmp_sel,
    pwm,
    ph1,
    ph2);
    output      pwm2;
    output      tc;
    input       clock;
    input       reset;
    output      pwm1;
    output      interrupt;
    input       capture;
    input       kill;
    input       enable;
    input       trigger;
    input       cmp_sel;
    output      pwm;
    output      ph1;
    output      ph2;

    parameter Resolution = 16;

          wire  Net_113;
          wire  Net_114;
          wire  Net_107;
          wire  Net_96;
          wire  Net_55;
          wire  Net_57;
          wire  Net_101;
          wire  Net_54;
          wire  Net_63;

    cy_psoc3_timer_v1_0 PWMHW (
        .timer_reset(reset),
        .capture(capture),
        .enable(Net_113),
        .kill(Net_107),
        .clock(clock),
        .tc(Net_63),
        .compare(Net_57),
        .interrupt(Net_54));

	// vmCompare (cy_virtualmux_v1_0)
	assign pwm = Net_57;

	// vmIRQ (cy_virtualmux_v1_0)
	assign interrupt = Net_54;

	// vmTC (cy_virtualmux_v1_0)
	assign tc = Net_63;

    OneTerminal OneTerminal_1 (
        .o(Net_113));

	// FFKillMux (cy_virtualmux_v1_0)
	assign Net_107 = kill;

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_114));



endmodule

// PWM_v2_20(CaptureMode=0, Compare1_16=false, Compare1_8=false, Compare2_16=false, Compare2_8=false, CompareStatusEdgeSense=true, CompareType1=1, CompareType1Software=0, CompareType2=1, CompareType2Software=0, CompareValue1=500, CompareValue2=63, CONTROL3=1, ControlReg=false, CtlModeReplacementString=SyncCtl, CyGetRegReplacementString=CY_GET_REG16, CySetRegReplacementString=CY_SET_REG16, DeadBand=0, DeadBand2_4=0, DeadBand256=0, DeadBandUsed=0, DeadTime=1, DitherOffset=0, EnableMode=0, FF16=true, FF8=false, FixedFunction=true, FixedFunctionUsed=1, InterruptOnCMP1=false, InterruptOnCMP2=false, InterruptOnKill=false, InterruptOnTC=false, IntOnCMP1=0, IntOnCMP2=0, IntOnKill=0, IntOnTC=0, KillMode=1, KillModeMinTime=0, MinimumKillTime=1, OneCompare=true, Period=999, PWMMode=0, PWMModeCenterAligned=0, RegDefReplacementString=reg16, RegSizeReplacementString=uint16, Resolution=16, RstStatusReplacementString=sSTSReg_rstSts, RunMode=0, Status=false, TriggerMode=0, UDB16=false, UDB8=false, UseControl=true, UseInterrupt=false, UseStatus=false, VerilogSectionReplacementString=sP16, CY_COMPONENT_NAME=PWM_v2_20, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=FL_HBridge, CY_INSTANCE_SHORT_NAME=FL_HBridge, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=20, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=FL_HBridge, )
module PWM_v2_20_3 (
    pwm2,
    tc,
    clock,
    reset,
    pwm1,
    interrupt,
    capture,
    kill,
    enable,
    trigger,
    cmp_sel,
    pwm,
    ph1,
    ph2);
    output      pwm2;
    output      tc;
    input       clock;
    input       reset;
    output      pwm1;
    output      interrupt;
    input       capture;
    input       kill;
    input       enable;
    input       trigger;
    input       cmp_sel;
    output      pwm;
    output      ph1;
    output      ph2;

    parameter Resolution = 16;

          wire  Net_113;
          wire  Net_114;
          wire  Net_107;
          wire  Net_96;
          wire  Net_55;
          wire  Net_57;
          wire  Net_101;
          wire  Net_54;
          wire  Net_63;

    cy_psoc3_timer_v1_0 PWMHW (
        .timer_reset(reset),
        .capture(capture),
        .enable(Net_113),
        .kill(Net_107),
        .clock(clock),
        .tc(Net_63),
        .compare(Net_57),
        .interrupt(Net_54));

	// vmCompare (cy_virtualmux_v1_0)
	assign pwm = Net_57;

	// vmIRQ (cy_virtualmux_v1_0)
	assign interrupt = Net_54;

	// vmTC (cy_virtualmux_v1_0)
	assign tc = Net_63;

    OneTerminal OneTerminal_1 (
        .o(Net_113));

	// FFKillMux (cy_virtualmux_v1_0)
	assign Net_107 = kill;

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_114));



endmodule

// PWM_v2_20(CaptureMode=0, Compare1_16=false, Compare1_8=false, Compare2_16=false, Compare2_8=false, CompareStatusEdgeSense=true, CompareType1=1, CompareType1Software=0, CompareType2=1, CompareType2Software=0, CompareValue1=500, CompareValue2=63, CONTROL3=1, ControlReg=false, CtlModeReplacementString=SyncCtl, CyGetRegReplacementString=CY_GET_REG16, CySetRegReplacementString=CY_SET_REG16, DeadBand=0, DeadBand2_4=0, DeadBand256=0, DeadBandUsed=0, DeadTime=1, DitherOffset=0, EnableMode=0, FF16=true, FF8=false, FixedFunction=true, FixedFunctionUsed=1, InterruptOnCMP1=false, InterruptOnCMP2=false, InterruptOnKill=false, InterruptOnTC=false, IntOnCMP1=0, IntOnCMP2=0, IntOnKill=0, IntOnTC=0, KillMode=1, KillModeMinTime=0, MinimumKillTime=1, OneCompare=true, Period=999, PWMMode=0, PWMModeCenterAligned=0, RegDefReplacementString=reg16, RegSizeReplacementString=uint16, Resolution=16, RstStatusReplacementString=sSTSReg_rstSts, RunMode=0, Status=false, TriggerMode=0, UDB16=false, UDB8=false, UseControl=true, UseInterrupt=false, UseStatus=false, VerilogSectionReplacementString=sP16, CY_COMPONENT_NAME=PWM_v2_20, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=BL_HBridge, CY_INSTANCE_SHORT_NAME=BL_HBridge, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=20, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=BL_HBridge, )
module PWM_v2_20_4 (
    pwm2,
    tc,
    clock,
    reset,
    pwm1,
    interrupt,
    capture,
    kill,
    enable,
    trigger,
    cmp_sel,
    pwm,
    ph1,
    ph2);
    output      pwm2;
    output      tc;
    input       clock;
    input       reset;
    output      pwm1;
    output      interrupt;
    input       capture;
    input       kill;
    input       enable;
    input       trigger;
    input       cmp_sel;
    output      pwm;
    output      ph1;
    output      ph2;

    parameter Resolution = 16;

          wire  Net_113;
          wire  Net_114;
          wire  Net_107;
          wire  Net_96;
          wire  Net_55;
          wire  Net_57;
          wire  Net_101;
          wire  Net_54;
          wire  Net_63;

    cy_psoc3_timer_v1_0 PWMHW (
        .timer_reset(reset),
        .capture(capture),
        .enable(Net_113),
        .kill(Net_107),
        .clock(clock),
        .tc(Net_63),
        .compare(Net_57),
        .interrupt(Net_54));

	// vmCompare (cy_virtualmux_v1_0)
	assign pwm = Net_57;

	// vmIRQ (cy_virtualmux_v1_0)
	assign interrupt = Net_54;

	// vmTC (cy_virtualmux_v1_0)
	assign tc = Net_63;

    OneTerminal OneTerminal_1 (
        .o(Net_113));

	// FFKillMux (cy_virtualmux_v1_0)
	assign Net_107 = kill;

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_114));



endmodule

// Component: B_Timer_v2_30
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_Timer_v2_30"
`include "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_Timer_v2_30\B_Timer_v2_30.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_Timer_v2_30"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_Timer_v2_30\B_Timer_v2_30.v"
`endif

// Timer_v2_30(CaptureAlternatingFall=false, CaptureAlternatingRise=false, CaptureCount=2, CaptureCounterEnabled=false, CaptureInputEnabled=true, CaptureMode=2, CONTROL3=0, ControlRegRemoved=0, CtlModeReplacementString=SyncCtl, CyGetRegReplacementString=CY_GET_REG16, CySetRegReplacementString=CY_SET_REG16, DeviceFamily=PSoC3, EnableMode=0, FF16=false, FF8=false, FixedFunction=false, FixedFunctionUsed=0, HWCaptureCounterEnabled=false, InterruptOnCapture=true, InterruptOnFIFOFull=false, InterruptOnTC=false, IntOnCapture=1, IntOnFIFOFull=0, IntOnTC=0, NumberOfCaptures=1, param45=1, Period=9999, RegDefReplacementString=reg16, RegSizeReplacementString=uint16, Resolution=16, RstStatusReplacementString=rstSts, RunMode=2, SiliconRevision=3, SoftwareCaptureModeEnabled=false, SoftwareTriggerModeEnabled=false, TriggerInputEnabled=true, TriggerMode=1, UDB16=true, UDB24=false, UDB32=false, UDB8=false, UDBControlReg=true, UsesHWEnable=0, VerilogSectionReplacementString=sT16, CY_COMPONENT_NAME=Timer_v2_30, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=TiltTime, CY_INSTANCE_SHORT_NAME=TiltTime, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=30, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=TiltTime, )
module Timer_v2_30_5 (
    clock,
    reset,
    interrupt,
    enable,
    capture,
    trigger,
    capture_out,
    tc);
    input       clock;
    input       reset;
    output      interrupt;
    input       enable;
    input       capture;
    input       trigger;
    output      capture_out;
    output      tc;

    parameter CaptureCount = 2;
    parameter CaptureCounterEnabled = 0;
    parameter DeviceFamily = "PSoC3";
    parameter InterruptOnCapture = 1;
    parameter InterruptOnTC = 0;
    parameter Resolution = 16;
    parameter SiliconRevision = "3";

          wire  Net_260;
          wire  Net_261;
          wire  Net_266;
          wire  Net_102;
          wire  Net_55;
          wire  Net_57;
          wire  Net_53;
          wire  Net_51;

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_260));

	// VirtualMux_2 (cy_virtualmux_v1_0)
	assign interrupt = Net_55;

	// VirtualMux_3 (cy_virtualmux_v1_0)
	assign tc = Net_53;

    B_Timer_v2_30 TimerUDB (
        .reset(reset),
        .interrupt(Net_55),
        .enable(enable),
        .trigger(trigger),
        .capture_in(capture),
        .capture_out(capture_out),
        .tc(Net_53),
        .clock(clock));
    defparam TimerUDB.Capture_Count = 2;
    defparam TimerUDB.CaptureCounterEnabled = 0;
    defparam TimerUDB.CaptureMode = 2;
    defparam TimerUDB.EnableMode = 0;
    defparam TimerUDB.InterruptOnCapture = 1;
    defparam TimerUDB.Resolution = 16;
    defparam TimerUDB.RunMode = 2;
    defparam TimerUDB.TriggerMode = 1;

    OneTerminal OneTerminal_1 (
        .o(Net_102));

	// VirtualMux_1 (cy_virtualmux_v1_0)
	assign Net_266 = Net_102;



endmodule

// Component: CyControlReg_v1_70
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\CyControlReg_v1_70"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\CyControlReg_v1_70\CyControlReg_v1_70.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\CyControlReg_v1_70"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\CyControlReg_v1_70\CyControlReg_v1_70.v"
`endif

// Timer_v2_30(CaptureAlternatingFall=false, CaptureAlternatingRise=false, CaptureCount=2, CaptureCounterEnabled=false, CaptureInputEnabled=true, CaptureMode=2, CONTROL3=0, ControlRegRemoved=0, CtlModeReplacementString=SyncCtl, CyGetRegReplacementString=CY_GET_REG16, CySetRegReplacementString=CY_SET_REG16, DeviceFamily=PSoC3, EnableMode=0, FF16=false, FF8=false, FixedFunction=false, FixedFunctionUsed=0, HWCaptureCounterEnabled=false, InterruptOnCapture=true, InterruptOnFIFOFull=false, InterruptOnTC=false, IntOnCapture=1, IntOnFIFOFull=0, IntOnTC=0, NumberOfCaptures=1, param45=1, Period=9999, RegDefReplacementString=reg16, RegSizeReplacementString=uint16, Resolution=16, RstStatusReplacementString=rstSts, RunMode=2, SiliconRevision=3, SoftwareCaptureModeEnabled=false, SoftwareTriggerModeEnabled=false, TriggerInputEnabled=true, TriggerMode=1, UDB16=true, UDB24=false, UDB32=false, UDB8=false, UDBControlReg=true, UsesHWEnable=0, VerilogSectionReplacementString=sT16, CY_COMPONENT_NAME=Timer_v2_30, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=PanTime, CY_INSTANCE_SHORT_NAME=PanTime, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=30, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=PanTime, )
module Timer_v2_30_6 (
    clock,
    reset,
    interrupt,
    enable,
    capture,
    trigger,
    capture_out,
    tc);
    input       clock;
    input       reset;
    output      interrupt;
    input       enable;
    input       capture;
    input       trigger;
    output      capture_out;
    output      tc;

    parameter CaptureCount = 2;
    parameter CaptureCounterEnabled = 0;
    parameter DeviceFamily = "PSoC3";
    parameter InterruptOnCapture = 1;
    parameter InterruptOnTC = 0;
    parameter Resolution = 16;
    parameter SiliconRevision = "3";

          wire  Net_260;
          wire  Net_261;
          wire  Net_266;
          wire  Net_102;
          wire  Net_55;
          wire  Net_57;
          wire  Net_53;
          wire  Net_51;

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_260));

	// VirtualMux_2 (cy_virtualmux_v1_0)
	assign interrupt = Net_55;

	// VirtualMux_3 (cy_virtualmux_v1_0)
	assign tc = Net_53;

    B_Timer_v2_30 TimerUDB (
        .reset(reset),
        .interrupt(Net_55),
        .enable(enable),
        .trigger(trigger),
        .capture_in(capture),
        .capture_out(capture_out),
        .tc(Net_53),
        .clock(clock));
    defparam TimerUDB.Capture_Count = 2;
    defparam TimerUDB.CaptureCounterEnabled = 0;
    defparam TimerUDB.CaptureMode = 2;
    defparam TimerUDB.EnableMode = 0;
    defparam TimerUDB.InterruptOnCapture = 1;
    defparam TimerUDB.Resolution = 16;
    defparam TimerUDB.RunMode = 2;
    defparam TimerUDB.TriggerMode = 1;

    OneTerminal OneTerminal_1 (
        .o(Net_102));

	// VirtualMux_1 (cy_virtualmux_v1_0)
	assign Net_266 = Net_102;



endmodule

// Component: cy_analog_virtualmux_v1_0
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\cy_analog_virtualmux_v1_0"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\cy_analog_virtualmux_v1_0\cy_analog_virtualmux_v1_0.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_analog_virtualmux_v1_0"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_analog_virtualmux_v1_0\cy_analog_virtualmux_v1_0.v"
`endif

// Component: AMux_v1_60
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\AMux_v1_60"
`include "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\AMux_v1_60\AMux_v1_60.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\AMux_v1_60"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\AMux_v1_60\AMux_v1_60.v"
`endif

// Component: cy_vref_v1_60
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\cy_vref_v1_60"
`include "$CYPRESS_DIR\..\psoc\content\cyprimitives\CyPrimitives.cylib\cy_vref_v1_60\cy_vref_v1_60.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_vref_v1_60"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cyprimitives\CyPrimitives.cylib\cy_vref_v1_60\cy_vref_v1_60.v"
`endif

// ADC_Vssa_v1_20(CY_COMPONENT_NAME=ADC_Vssa_v1_20, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=ADC_Vssa_2, CY_INSTANCE_SHORT_NAME=ADC_Vssa_2, CY_MAJOR_VERSION=1, CY_MINOR_VERSION=20, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=ADC_ADC_Vssa_2, )
module ADC_Vssa_v1_20_7 (
    vout);
    inout       vout;
    electrical  vout;




	cy_vref_v1_0
		#(.autoenable(1),
		  .guid("15B3DB15-B7B3-4d62-A2DF-25EA392A7161"),
		  .name("Vssa (GND)"))
		vRef_1
		 (.vout(vout));




endmodule

// ADC_Vssa_v1_20(CY_COMPONENT_NAME=ADC_Vssa_v1_20, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=ADC_Vssa_1, CY_INSTANCE_SHORT_NAME=ADC_Vssa_1, CY_MAJOR_VERSION=1, CY_MINOR_VERSION=20, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=ADC_ADC_Vssa_1, )
module ADC_Vssa_v1_20_8 (
    vout);
    inout       vout;
    electrical  vout;




	cy_vref_v1_0
		#(.autoenable(1),
		  .guid("15B3DB15-B7B3-4d62-A2DF-25EA392A7161"),
		  .name("Vssa (GND)"))
		vRef_1
		 (.vout(vout));




endmodule

// ADC_DelSig_v2_30(ADC_Charge_Pump_Clock=true, ADC_Clock=1, ADC_CLOCK_FREQUENCY=128000, ADC_Input_Mode=1, ADC_Input_Range=2, ADC_Input_Range_Config2=0, ADC_Input_Range_Config3=0, ADC_Input_Range_Config4=0, ADC_Power=1, ADC_Reference=5, ADC_Reference_Config2=0, ADC_Reference_Config3=0, ADC_Reference_Config4=0, ADC_Resolution=16, ADC_Resolution_Config2=16, ADC_Resolution_Config3=16, ADC_Resolution_Config4=16, Clock_Frequency=64000, Comment_Config1=Default Config, Comment_Config2=Second Config, Comment_Config3=Third Config, Comment_Config4=Fourth Config, Config1_Name=CFG1, Config2_Name=CFG2, Config3_Name=CFG3, Config4_Name=CFG4, Configs=4, Conversion_Mode=2, Conversion_Mode_Config2=2, Conversion_Mode_Config3=2, Conversion_Mode_Config4=2, Debug=false, DsmName=DSM4, Enable_Vref_Vss=false, Input_Buffer_Gain=1, Input_Buffer_Gain_Config2=1, Input_Buffer_Gain_Config3=1, Input_Buffer_Gain_Config4=1, Input_Buffer_Mode=1, Input_Buffer_Mode_Config2=1, Input_Buffer_Mode_Config3=1, Input_Buffer_Mode_Config4=1, PSOC5A=false, Ref_Voltage=1.25, Ref_Voltage_Config2=1.024, Ref_Voltage_Config3=1.024, Ref_Voltage_Config4=1.024, Sample_Rate=2000, Sample_Rate_Config2=10000, Sample_Rate_Config3=10000, Sample_Rate_Config4=10000, sRate_Err=false, Start_of_Conversion=0, Vdda_Value=5, CY_COMPONENT_NAME=ADC_DelSig_v2_30, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=ADC, CY_INSTANCE_SHORT_NAME=ADC, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=30, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=ADC, )
module ADC_DelSig_v2_30_9 (
    vplus,
    vminus,
    soc,
    eoc,
    aclk,
    nVref);
    inout       vplus;
    electrical  vplus;
    inout       vminus;
    electrical  vminus;
    input       soc;
    output      eoc;
    input       aclk;
    inout       nVref;
    electrical  nVref;


          wire  aclock;
          wire [3:0] mod_dat;
          wire  mod_reset;
          wire  Net_1;
    electrical  Net_2;
    electrical  Net_3;
          wire  Net_4;
          wire [7:0] Net_5;
          wire  Net_6;
          wire  Net_7;
          wire  Net_8;
          wire [7:0] Net_9;
          wire  Net_10;
    electrical  Net_11;
    electrical  Net_12;
    electrical  Net_13;
    electrical  Net_686;
    electrical  Net_690;
    electrical  Net_681;
    electrical  Net_677;
    electrical  Net_570;
          wire  Net_488;
          wire  Net_482;
          wire  Net_481;
          wire  Net_478;
          wire  Net_438;
          wire [3:0] Net_471;
          wire [3:0] Net_470;
    electrical  Net_352;
    electrical  Net_580;
    electrical  Net_349;
    electrical  Net_573;
    electrical  Net_257;
          wire  Net_487;
          wire  Net_40;
    electrical  Net_520;

    cy_psoc3_decimator_v1_0 DEC (
        .aclock(aclock),
        .mod_dat(mod_dat[3:0]),
        .ext_start(soc),
        .mod_reset(mod_reset),
        .interrupt(eoc));

    ZeroTerminal ZeroTerminal_1 (
        .z(Net_481));

    ZeroTerminal ZeroTerminal_2 (
        .z(Net_482));


	cy_clock_v1_0
		#(.id("d4bbb96a-d746-4ce5-b6f2-a5948cd1ffb4/edd15f43-b66b-457b-be3a-5342345270c8"),
		  .source_clock_id("61737EF6-3B74-48f9-8B91-F7473A442AE7"),
		  .divisor(0),
		  .period("7812500000"),
		  .is_direct(0),
		  .is_digital(0))
		theACLK
		 (.clock_out(Net_40));


	// Clock_VirtualMux (cy_virtualmux_v1_0)
	assign Net_488 = Net_40;


	cy_isr_v1_0
		#(.int_type(2'b10))
		IRQ
		 (.int_signal(eoc));


	// cy_analog_virtualmux_1 (cy_analog_virtualmux_v1_0)
	cy_connect_v1_0 cy_analog_virtualmux_1_connect(Net_520, Net_681);
	defparam cy_analog_virtualmux_1_connect.sig_width = 1;

	// cy_analog_virtualmux_2 (cy_analog_virtualmux_v1_0)
	cy_connect_v1_0 cy_analog_virtualmux_2_connect(Net_580, Net_349);
	defparam cy_analog_virtualmux_2_connect.sig_width = 1;

	// cy_analog_virtualmux_3 (cy_analog_virtualmux_v1_0)
	cy_connect_v1_0 cy_analog_virtualmux_3_connect(Net_573, Net_257);
	defparam cy_analog_virtualmux_3_connect.sig_width = 1;

    cy_analog_noconnect_v1_0 cy_analog_noconnect_2 (
        .noconnect(Net_349));

    cy_analog_noconnect_v1_0 cy_analog_noconnect_3 (
        .noconnect(Net_257));


	cy_clock_v1_0
		#(.id("d4bbb96a-d746-4ce5-b6f2-a5948cd1ffb4/b7604721-db56-4477-98c2-8fae77869066"),
		  .source_clock_id("61737EF6-3B74-48f9-8B91-F7473A442AE7"),
		  .divisor(0),
		  .period("1000000000"),
		  .is_direct(0),
		  .is_digital(1))
		Ext_CP_Clk
		 (.clock_out(Net_487));


	// cy_analog_virtualmux_5 (cy_analog_virtualmux_v1_0)
	cy_connect_v1_0 cy_analog_virtualmux_5_connect(Net_570, Net_352);
	defparam cy_analog_virtualmux_5_connect.sig_width = 1;

    cy_analog_noconnect_v1_0 cy_analog_noconnect_5 (
        .noconnect(Net_352));

    cy_psoc3_ds_mod_v4_0 DSM4 (
        .vplus(vplus),
        .vminus(Net_520),
        .modbit(Net_7),
        .reset_udb(Net_8),
        .aclock(Net_488),
        .mod_dat(Net_471[3:0]),
        .dout_udb(Net_9[7:0]),
        .reset_dec(mod_reset),
        .dec_clock(Net_478),
        .extclk_cp_udb(Net_487),
        .clk_udb(1'b0),
        .ext_pin_1(Net_580),
        .ext_pin_2(Net_573),
        .ext_vssa(Net_570),
        .qtz_ref(Net_677));
    defparam DSM4.resolution = 16;

    ZeroTerminal ZeroTerminal_3 (
        .z(Net_7));

    ZeroTerminal ZeroTerminal_5 (
        .z(Net_8));

	// Clock_VirtualMux_2 (cy_virtualmux_v1_0)
	assign mod_dat[3:0] = Net_471[3:0];

	// Clock_VirtualMux_3 (cy_virtualmux_v1_0)
	assign aclock = Net_478;

    // -- AMux AMux start -- ***
    // -- Mux A --
    
    cy_psoc3_amux_v1_0 AMux(
        .muxin({
            Net_690,
            Net_11
            }),
        .vout(Net_681)
        );
    
    defparam AMux.muxin_width = 2;
    defparam AMux.init_mux_sel = 2'h0;
    defparam AMux.one_active = 0;
    defparam AMux.connect_mode = 1;
    
    // -- AMux AMux end --

	// cy_analog_virtualmux_4 (cy_analog_virtualmux_v1_0)
	cy_connect_v1_0 cy_analog_virtualmux_4_connect(Net_677, Net_12);
	defparam cy_analog_virtualmux_4_connect.sig_width = 1;

    cy_analog_noconnect_v1_0 cy_analog_noconnect_1 (
        .noconnect(Net_12));

	// cy_analog_virtualmux_6 (cy_analog_virtualmux_v1_0)
	cy_connect_v1_0 cy_analog_virtualmux_6_connect(Net_690, Net_13);
	defparam cy_analog_virtualmux_6_connect.sig_width = 1;

    ADC_Vssa_v1_20_7 ADC_Vssa_2 (
        .vout(Net_13));

    ADC_Vssa_v1_20_8 ADC_Vssa_1 (
        .vout(Net_11));



endmodule

// Component: B_UART_v2_20
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_UART_v2_20"
`include "$CYPRESS_DIR\..\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_UART_v2_20\B_UART_v2_20.v"
`else
`define CY_BLK_DIR "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_UART_v2_20"
`include "C:\Program Files\Cypress\PSoC Creator\2.1\PSoC Creator\psoc\content\cycomponentlibrary\CyComponentLibrary.cylib\B_UART_v2_20\B_UART_v2_20.v"
`endif

// UART_v2_20(Address1=0, Address2=0, BaudRate=19200, BreakBitsRX=13, BreakBitsTX=13, BreakDetect=false, CRCoutputsEn=false, CtrlModeReplacementString=SyncCtl, Enable_RX=1, Enable_RXIntInterrupt=0, Enable_TX=0, Enable_TXIntInterrupt=0, EnableHWAddress=0, EnIntRXInterrupt=false, EnIntTXInterrupt=false, FlowControl=0, HalfDuplexEn=false, HwTXEnSignal=true, InternalClock=true, InternalClockUsed=1, InterruptOnAddDetect=0, InterruptOnAddressMatch=0, InterruptOnBreak=0, InterruptOnByteRcvd=1, InterruptOnOverrunError=0, InterruptOnParityError=0, InterruptOnStopError=0, InterruptOnTXComplete=false, InterruptOnTXFifoEmpty=false, InterruptOnTXFifoFull=false, InterruptOnTXFifoNotFull=false, IntOnAddressDetect=false, IntOnAddressMatch=false, IntOnBreak=false, IntOnByteRcvd=true, IntOnOverrunError=false, IntOnParityError=false, IntOnStopError=false, NumDataBits=8, NumStopBits=1, OverSamplingRate=8, ParityType=0, ParityTypeSw=false, RequiredClock=153600, RXAddressMode=0, RXBufferSize=4, RxBuffRegSizeReplacementString=uint8, RXEnable=true, TXBitClkGenDP=true, TXBufferSize=4, TxBuffRegSizeReplacementString=uint8, TXEnable=false, Use23Polling=true, CY_COMPONENT_NAME=UART_v2_20, CY_CONTROL_FILE=<:default:>, CY_FITTER_NAME=Pixy_UART, CY_INSTANCE_SHORT_NAME=Pixy_UART, CY_MAJOR_VERSION=2, CY_MINOR_VERSION=20, CY_REMOVE=false, CY_SUPPRESS_API_GEN=false, CY_VERSION=cydsfit No Version Information Found, INSTANCE_NAME=Pixy_UART, )
module UART_v2_20_10 (
    rx_clk,
    rx_data,
    tx_clk,
    tx_data,
    rx_interrupt,
    tx_interrupt,
    tx,
    tx_en,
    rts_n,
    reset,
    cts_n,
    clock,
    rx);
    output      rx_clk;
    output      rx_data;
    output      tx_clk;
    output      tx_data;
    output      rx_interrupt;
    output      tx_interrupt;
    output      tx;
    output      tx_en;
    output      rts_n;
    input       reset;
    input       cts_n;
    input       clock;
    input       rx;

    parameter Address1 = 0;
    parameter Address2 = 0;
    parameter EnIntRXInterrupt = 0;
    parameter EnIntTXInterrupt = 0;
    parameter FlowControl = 0;
    parameter HalfDuplexEn = 0;
    parameter HwTXEnSignal = 1;
    parameter NumDataBits = 8;
    parameter NumStopBits = 1;
    parameter ParityType = 0;
    parameter RXEnable = 1;
    parameter TXEnable = 0;

          wire  Net_289;
          wire  Net_61;
          wire  Net_9;


	cy_clock_v1_0
		#(.id("b0162966-0060-4af5-82d1-fcb491ad7619/be0a0e37-ad17-42ca-b5a1-1a654d736358"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("6510416666.66667"),
		  .is_direct(0),
		  .is_digital(1))
		IntClock
		 (.clock_out(Net_9));


	// VirtualMux_1 (cy_virtualmux_v1_0)
	assign Net_61 = Net_9;

    B_UART_v2_20 BUART (
        .cts_n(cts_n),
        .tx(tx),
        .rts_n(rts_n),
        .tx_en(tx_en),
        .clock(Net_61),
        .reset(reset),
        .rx(rx),
        .tx_interrupt(tx_interrupt),
        .rx_interrupt(rx_interrupt),
        .tx_data(tx_data),
        .tx_clk(tx_clk),
        .rx_data(rx_data),
        .rx_clk(rx_clk));
    defparam BUART.Address1 = 0;
    defparam BUART.Address2 = 0;
    defparam BUART.BreakBitsRX = 13;
    defparam BUART.BreakBitsTX = 13;
    defparam BUART.BreakDetect = 0;
    defparam BUART.CRCoutputsEn = 0;
    defparam BUART.FlowControl = 0;
    defparam BUART.HalfDuplexEn = 0;
    defparam BUART.HwTXEnSignal = 1;
    defparam BUART.NumDataBits = 8;
    defparam BUART.NumStopBits = 1;
    defparam BUART.OverSampleCount = 8;
    defparam BUART.ParityType = 0;
    defparam BUART.ParityTypeSw = 0;
    defparam BUART.RXAddressMode = 0;
    defparam BUART.RXEnable = 1;
    defparam BUART.RXStatusIntEnable = 1;
    defparam BUART.TXBitClkGenDP = 1;
    defparam BUART.TXEnable = 0;
    defparam BUART.Use23Polling = 1;



endmodule

// top
module top ;

          wire  Net_747;
          wire  Net_746;
          wire  Net_745;
          wire  Net_744;
          wire  Net_742;
          wire  Net_740;
          wire  Net_739;
          wire  Net_738;
          wire  Net_737;
          wire  Net_736;
          wire  Net_717;
          wire  Net_741;
          wire  Net_753;
          wire  Net_743;
    electrical  Net_511;
          wire  Net_510;
          wire  Net_505;
          wire  Net_509;
    electrical  Net_508;
    electrical  Net_496;
          wire  Net_442;
          wire  Net_450;
          wire  Net_449;
          wire  Net_448;
          wire  Net_447;
          wire  Net_446;
          wire  Net_445;
          wire  Net_444;
          wire  Net_443;
          wire  Net_418;
          wire  Net_385;
          wire  Net_384;
          wire  Net_383;
          wire  Net_486;
          wire  Net_415;
          wire  Net_413;
          wire  Net_412;
          wire  Net_411;
          wire  Net_410;
          wire  Net_408;
          wire  Net_407;
          wire  Net_406;
          wire  Net_439;
          wire  Net_438;
          wire  Net_437;
          wire  Net_421;
          wire  Net_378;
          wire  Net_488;
          wire  Net_221;
          wire  Net_220;
          wire  Net_219;
          wire  Net_218;
          wire  Net_217;
          wire  Net_192;
          wire  Net_216;
          wire  Net_215;
          wire  Net_214;
          wire  Net_213;
          wire  Net_212;
          wire  Net_185;
          wire  Net_211;
          wire  Net_210;
          wire  Net_209;
          wire  Net_208;
          wire  Net_207;
          wire  Net_177;
          wire  Net_206;
          wire  Net_205;
          wire  Net_204;
          wire  Net_203;
          wire  Net_202;
          wire  Net_170;
          wire  Net_168;
          wire  Net_167;
          wire  Net_166;
          wire  Net_165;
          wire  Net_164;
          wire  Net_151;
          wire  Net_163;
          wire  Net_162;
          wire  Net_161;
          wire  Net_160;
          wire  Net_159;
          wire  Net_144;
          wire  Net_138;
          wire  Net_136;
          wire  Net_135;
          wire  Net_133;
          wire  Net_132;
          wire  Net_131;
          wire  Net_130;
          wire  Net_129;
          wire  Net_128;
          wire  Net_127;
          wire  Net_126;
          wire  Net_125;
          wire  Net_124;
          wire  Net_748;
          wire  Net_30;
          wire  Net_10;
          wire  Net_434;
          wire  Net_419;
          wire  Net_420;
          wire  Net_425;
          wire  Net_414;
          wire  Net_292;
          wire  Net_12;
          wire  Net_196;
          wire  Net_181;
          wire  Net_155;
          wire  Net_134;

    PWM_v2_20_0 FR_HBridge (
        .reset(Net_124),
        .clock(Net_30),
        .tc(Net_125),
        .pwm1(Net_126),
        .pwm2(Net_127),
        .interrupt(Net_128),
        .capture(1'b0),
        .kill(Net_130),
        .enable(1'b1),
        .trigger(1'b0),
        .cmp_sel(1'b0),
        .pwm(Net_134),
        .ph1(Net_135),
        .ph2(Net_136));
    defparam FR_HBridge.Resolution = 16;


	cy_clock_v1_0
		#(.id("446b6533-a247-49b7-8127-869ffb035667"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("125000000"),
		  .is_direct(0),
		  .is_digital(1))
		PWM_clk
		 (.clock_out(Net_30));


    ZeroTerminal ZeroTerminal_1 (
        .z(Net_124));

    ZeroTerminal ZeroTerminal_2 (
        .z(Net_130));

	wire [0:0] tmpOE__FR_H2_net;
	wire [0:0] tmpFB_0__FR_H2_net;
	wire [0:0] tmpIO_0__FR_H2_net;
	wire [0:0] tmpINTERRUPT_0__FR_H2_net;
	electrical [0:0] tmpSIOVREF__FR_H2_net;

	cy_psoc3_pins_v1_10
		#(.id("52f31aa9-2f0a-497d-9a1f-1424095e13e6"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		FR_H2
		 (.oe(tmpOE__FR_H2_net),
		  .y({Net_134}),
		  .fb({tmpFB_0__FR_H2_net[0:0]}),
		  .io({tmpIO_0__FR_H2_net[0:0]}),
		  .siovref(tmpSIOVREF__FR_H2_net),
		  .interrupt({tmpINTERRUPT_0__FR_H2_net[0:0]}));

	assign tmpOE__FR_H2_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	wire [0:0] tmpOE__FR_H_net;
	wire [0:0] tmpFB_0__FR_H_net;
	wire [0:0] tmpIO_0__FR_H_net;
	wire [0:0] tmpINTERRUPT_0__FR_H_net;
	electrical [0:0] tmpSIOVREF__FR_H_net;

	cy_psoc3_pins_v1_10
		#(.id("0ab010de-d445-4daf-9c0c-9d62d09af349"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		FR_H
		 (.oe(tmpOE__FR_H_net),
		  .y({Net_134}),
		  .fb({tmpFB_0__FR_H_net[0:0]}),
		  .io({tmpIO_0__FR_H_net[0:0]}),
		  .siovref(tmpSIOVREF__FR_H_net),
		  .interrupt({tmpINTERRUPT_0__FR_H_net[0:0]}));

	assign tmpOE__FR_H_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

    CharLCD_v1_60_1 LCD ();

    PWM_v2_20_2 BR_HBridge (
        .reset(Net_144),
        .clock(Net_30),
        .tc(Net_159),
        .pwm1(Net_160),
        .pwm2(Net_161),
        .interrupt(Net_162),
        .capture(1'b0),
        .kill(Net_151),
        .enable(1'b1),
        .trigger(1'b0),
        .cmp_sel(1'b0),
        .pwm(Net_155),
        .ph1(Net_167),
        .ph2(Net_168));
    defparam BR_HBridge.Resolution = 16;

    ZeroTerminal ZeroTerminal_3 (
        .z(Net_144));

    ZeroTerminal ZeroTerminal_4 (
        .z(Net_151));

	wire [0:0] tmpOE__BR_H2_net;
	wire [0:0] tmpFB_0__BR_H2_net;
	wire [0:0] tmpIO_0__BR_H2_net;
	wire [0:0] tmpINTERRUPT_0__BR_H2_net;
	electrical [0:0] tmpSIOVREF__BR_H2_net;

	cy_psoc3_pins_v1_10
		#(.id("6ff361e5-0cb8-47a0-b04a-c5248f8facbf"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		BR_H2
		 (.oe(tmpOE__BR_H2_net),
		  .y({Net_155}),
		  .fb({tmpFB_0__BR_H2_net[0:0]}),
		  .io({tmpIO_0__BR_H2_net[0:0]}),
		  .siovref(tmpSIOVREF__BR_H2_net),
		  .interrupt({tmpINTERRUPT_0__BR_H2_net[0:0]}));

	assign tmpOE__BR_H2_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	wire [0:0] tmpOE__BR_H_net;
	wire [0:0] tmpFB_0__BR_H_net;
	wire [0:0] tmpIO_0__BR_H_net;
	wire [0:0] tmpINTERRUPT_0__BR_H_net;
	electrical [0:0] tmpSIOVREF__BR_H_net;

	cy_psoc3_pins_v1_10
		#(.id("c118475c-dfc9-4fc1-bd29-fa10139917cf"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		BR_H
		 (.oe(tmpOE__BR_H_net),
		  .y({Net_155}),
		  .fb({tmpFB_0__BR_H_net[0:0]}),
		  .io({tmpIO_0__BR_H_net[0:0]}),
		  .siovref(tmpSIOVREF__BR_H_net),
		  .interrupt({tmpINTERRUPT_0__BR_H_net[0:0]}));

	assign tmpOE__BR_H_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

    PWM_v2_20_3 FL_HBridge (
        .reset(Net_170),
        .clock(Net_30),
        .tc(Net_202),
        .pwm1(Net_203),
        .pwm2(Net_204),
        .interrupt(Net_205),
        .capture(1'b0),
        .kill(Net_177),
        .enable(1'b1),
        .trigger(1'b0),
        .cmp_sel(1'b0),
        .pwm(Net_181),
        .ph1(Net_210),
        .ph2(Net_211));
    defparam FL_HBridge.Resolution = 16;

    ZeroTerminal ZeroTerminal_5 (
        .z(Net_170));

    ZeroTerminal ZeroTerminal_6 (
        .z(Net_177));

	wire [0:0] tmpOE__FL_H2_net;
	wire [0:0] tmpFB_0__FL_H2_net;
	wire [0:0] tmpIO_0__FL_H2_net;
	wire [0:0] tmpINTERRUPT_0__FL_H2_net;
	electrical [0:0] tmpSIOVREF__FL_H2_net;

	cy_psoc3_pins_v1_10
		#(.id("cbf76003-18e8-4e4e-ab5d-d5ddf47a2c8f"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		FL_H2
		 (.oe(tmpOE__FL_H2_net),
		  .y({Net_181}),
		  .fb({tmpFB_0__FL_H2_net[0:0]}),
		  .io({tmpIO_0__FL_H2_net[0:0]}),
		  .siovref(tmpSIOVREF__FL_H2_net),
		  .interrupt({tmpINTERRUPT_0__FL_H2_net[0:0]}));

	assign tmpOE__FL_H2_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	wire [0:0] tmpOE__FL_H_net;
	wire [0:0] tmpFB_0__FL_H_net;
	wire [0:0] tmpIO_0__FL_H_net;
	wire [0:0] tmpINTERRUPT_0__FL_H_net;
	electrical [0:0] tmpSIOVREF__FL_H_net;

	cy_psoc3_pins_v1_10
		#(.id("b53863fa-4164-4ce0-adab-14acbd519874"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		FL_H
		 (.oe(tmpOE__FL_H_net),
		  .y({Net_181}),
		  .fb({tmpFB_0__FL_H_net[0:0]}),
		  .io({tmpIO_0__FL_H_net[0:0]}),
		  .siovref(tmpSIOVREF__FL_H_net),
		  .interrupt({tmpINTERRUPT_0__FL_H_net[0:0]}));

	assign tmpOE__FL_H_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

    PWM_v2_20_4 BL_HBridge (
        .reset(Net_185),
        .clock(Net_30),
        .tc(Net_212),
        .pwm1(Net_213),
        .pwm2(Net_214),
        .interrupt(Net_215),
        .capture(1'b0),
        .kill(Net_192),
        .enable(1'b1),
        .trigger(1'b0),
        .cmp_sel(1'b0),
        .pwm(Net_196),
        .ph1(Net_220),
        .ph2(Net_221));
    defparam BL_HBridge.Resolution = 16;

    ZeroTerminal ZeroTerminal_7 (
        .z(Net_185));

    ZeroTerminal ZeroTerminal_8 (
        .z(Net_192));


	cy_clock_v1_0
		#(.id("841fb2d0-3f05-40af-8760-a480e4b670cc"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("1000000000"),
		  .is_direct(0),
		  .is_digital(1))
		timer_clock_1
		 (.clock_out(Net_425));


	wire [0:0] tmpOE__BL_H2_net;
	wire [0:0] tmpFB_0__BL_H2_net;
	wire [0:0] tmpIO_0__BL_H2_net;
	wire [0:0] tmpINTERRUPT_0__BL_H2_net;
	electrical [0:0] tmpSIOVREF__BL_H2_net;

	cy_psoc3_pins_v1_10
		#(.id("9cbbf6fa-1c5c-4ccf-96c5-064673c1c04f"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		BL_H2
		 (.oe(tmpOE__BL_H2_net),
		  .y({Net_196}),
		  .fb({tmpFB_0__BL_H2_net[0:0]}),
		  .io({tmpIO_0__BL_H2_net[0:0]}),
		  .siovref(tmpSIOVREF__BL_H2_net),
		  .interrupt({tmpINTERRUPT_0__BL_H2_net[0:0]}));

	assign tmpOE__BL_H2_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	wire [0:0] tmpOE__BL_H_net;
	wire [0:0] tmpFB_0__BL_H_net;
	wire [0:0] tmpIO_0__BL_H_net;
	wire [0:0] tmpINTERRUPT_0__BL_H_net;
	electrical [0:0] tmpSIOVREF__BL_H_net;

	cy_psoc3_pins_v1_10
		#(.id("9af6b58f-5152-4192-ae3e-30795ca8efc9"),
		  .drive_mode(3'b110),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b1),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("O"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		BL_H
		 (.oe(tmpOE__BL_H_net),
		  .y({Net_196}),
		  .fb({tmpFB_0__BL_H_net[0:0]}),
		  .io({tmpIO_0__BL_H_net[0:0]}),
		  .siovref(tmpSIOVREF__BL_H_net),
		  .interrupt({tmpINTERRUPT_0__BL_H_net[0:0]}));

	assign tmpOE__BL_H_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

	wire [0:0] tmpOE__PixyPan_net;
	wire [0:0] tmpIO_0__PixyPan_net;
	wire [0:0] tmpINTERRUPT_0__PixyPan_net;
	electrical [0:0] tmpSIOVREF__PixyPan_net;

	cy_psoc3_pins_v1_10
		#(.id("4c15b41e-e284-4978-99e7-5aaee19bd0ce"),
		  .drive_mode(3'b001),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b0),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("I"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b00),
		  .width(1))
		PixyPan
		 (.oe(tmpOE__PixyPan_net),
		  .y({1'b0}),
		  .fb({Net_292}),
		  .io({tmpIO_0__PixyPan_net[0:0]}),
		  .siovref(tmpSIOVREF__PixyPan_net),
		  .interrupt({tmpINTERRUPT_0__PixyPan_net[0:0]}));

	assign tmpOE__PixyPan_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};


	cy_isr_v1_0
		#(.int_type(2'b00))
		Pan_int
		 (.int_signal(Net_378));


	wire [0:0] tmpOE__PixyTilt_net;
	wire [0:0] tmpIO_0__PixyTilt_net;
	wire [0:0] tmpINTERRUPT_0__PixyTilt_net;
	electrical [0:0] tmpSIOVREF__PixyTilt_net;

	cy_psoc3_pins_v1_10
		#(.id("602b8978-65fe-4096-bc91-ebc3c6107f11"),
		  .drive_mode(3'b001),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b0),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("I"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b00),
		  .width(1))
		PixyTilt
		 (.oe(tmpOE__PixyTilt_net),
		  .y({1'b0}),
		  .fb({Net_419}),
		  .io({tmpIO_0__PixyTilt_net[0:0]}),
		  .siovref(tmpSIOVREF__PixyTilt_net),
		  .interrupt({tmpINTERRUPT_0__PixyTilt_net[0:0]}));

	assign tmpOE__PixyTilt_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

    Timer_v2_30_5 TiltTime (
        .reset(Net_420),
        .interrupt(Net_421),
        .enable(1'b1),
        .trigger(Net_419),
        .capture(Net_419),
        .capture_out(Net_438),
        .tc(Net_439),
        .clock(Net_425));
    defparam TiltTime.CaptureCount = 2;
    defparam TiltTime.CaptureCounterEnabled = 0;
    defparam TiltTime.DeviceFamily = "PSoC3";
    defparam TiltTime.InterruptOnCapture = 1;
    defparam TiltTime.InterruptOnTC = 0;
    defparam TiltTime.Resolution = 16;
    defparam TiltTime.SiliconRevision = "3";

    CyControlReg_v1_70 Pan_Reset (
        .control_1(Net_406),
        .control_2(Net_407),
        .control_3(Net_408),
        .control_0(Net_12),
        .control_4(Net_410),
        .control_5(Net_411),
        .control_6(Net_412),
        .control_7(Net_413),
        .clock(Net_414),
        .reset(1'b0));
    defparam Pan_Reset.Bit0Mode = 3;
    defparam Pan_Reset.Bit1Mode = 0;
    defparam Pan_Reset.Bit2Mode = 0;
    defparam Pan_Reset.Bit3Mode = 0;
    defparam Pan_Reset.Bit4Mode = 0;
    defparam Pan_Reset.Bit5Mode = 0;
    defparam Pan_Reset.Bit6Mode = 0;
    defparam Pan_Reset.Bit7Mode = 0;
    defparam Pan_Reset.BitValue = 0;
    defparam Pan_Reset.BusDisplay = 0;
    defparam Pan_Reset.ExtrReset = 1;
    defparam Pan_Reset.NumOutputs = 1;


	cy_clock_v1_0
		#(.id("920ac626-75fc-42be-bddc-386ba9cec7f2"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("1000000000"),
		  .is_direct(0),
		  .is_digital(1))
		timer_clock
		 (.clock_out(Net_10));


    Timer_v2_30_6 PanTime (
        .reset(Net_12),
        .interrupt(Net_378),
        .enable(1'b1),
        .trigger(Net_292),
        .capture(Net_292),
        .capture_out(Net_384),
        .tc(Net_385),
        .clock(Net_10));
    defparam PanTime.CaptureCount = 2;
    defparam PanTime.CaptureCounterEnabled = 0;
    defparam PanTime.DeviceFamily = "PSoC3";
    defparam PanTime.InterruptOnCapture = 1;
    defparam PanTime.InterruptOnTC = 0;
    defparam PanTime.Resolution = 16;
    defparam PanTime.SiliconRevision = "3";


	cy_clock_v1_0
		#(.id("c6dd7623-a85d-4014-982f-7ec3fead88eb"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("1000000000"),
		  .is_direct(0),
		  .is_digital(1))
		pan_reset_clk
		 (.clock_out(Net_414));



	cy_isr_v1_0
		#(.int_type(2'b00))
		Tilt_int
		 (.int_signal(Net_421));


    CyControlReg_v1_70 Tilt_Reset (
        .control_1(Net_443),
        .control_2(Net_444),
        .control_3(Net_445),
        .control_0(Net_420),
        .control_4(Net_446),
        .control_5(Net_447),
        .control_6(Net_448),
        .control_7(Net_449),
        .clock(Net_434),
        .reset(1'b0));
    defparam Tilt_Reset.Bit0Mode = 3;
    defparam Tilt_Reset.Bit1Mode = 0;
    defparam Tilt_Reset.Bit2Mode = 0;
    defparam Tilt_Reset.Bit3Mode = 0;
    defparam Tilt_Reset.Bit4Mode = 0;
    defparam Tilt_Reset.Bit5Mode = 0;
    defparam Tilt_Reset.Bit6Mode = 0;
    defparam Tilt_Reset.Bit7Mode = 0;
    defparam Tilt_Reset.BitValue = 0;
    defparam Tilt_Reset.BusDisplay = 0;
    defparam Tilt_Reset.ExtrReset = 1;
    defparam Tilt_Reset.NumOutputs = 1;


	cy_clock_v1_0
		#(.id("3c75eb42-6141-41a2-a420-c3c6b498cb85"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("1000000000"),
		  .is_direct(0),
		  .is_digital(1))
		tilt_reset_clk
		 (.clock_out(Net_434));



	cy_isr_v1_0
		#(.int_type(2'b00))
		Clk_int
		 (.int_signal(Net_748));


	wire [0:0] tmpOE__Gyro_Pin_net;
	wire [0:0] tmpFB_0__Gyro_Pin_net;
	wire [0:0] tmpIO_0__Gyro_Pin_net;
	wire [0:0] tmpINTERRUPT_0__Gyro_Pin_net;
	electrical [0:0] tmpSIOVREF__Gyro_Pin_net;

	cy_psoc3_pins_v1_10
		#(.id("05a9c8de-3ba2-4909-8250-95fdc61c0bf4"),
		  .drive_mode(3'b000),
		  .ibuf_enabled(1'b0),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b0),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("A"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b10),
		  .width(1))
		Gyro_Pin
		 (.oe(tmpOE__Gyro_Pin_net),
		  .y({1'b0}),
		  .fb({tmpFB_0__Gyro_Pin_net[0:0]}),
		  .analog({Net_496}),
		  .io({tmpIO_0__Gyro_Pin_net[0:0]}),
		  .siovref(tmpSIOVREF__Gyro_Pin_net),
		  .interrupt({tmpINTERRUPT_0__Gyro_Pin_net[0:0]}));

	assign tmpOE__Gyro_Pin_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

    ADC_DelSig_v2_30_9 ADC (
        .vplus(Net_496),
        .vminus(Net_508),
        .soc(1'b1),
        .eoc(Net_505),
        .aclk(1'b0),
        .nVref(Net_511));


	cy_isr_v1_0
		#(.int_type(2'b00))
		Analog_int
		 (.int_signal(Net_505));



	cy_isr_v1_0
		#(.int_type(2'b00))
		Pixy_int
		 (.int_signal(Net_743));



	cy_clock_v1_0
		#(.id("e4d0b4f2-87ad-4cdf-be5b-24faaaf7dfad"),
		  .source_clock_id(""),
		  .divisor(0),
		  .period("20000000000000"),
		  .is_direct(0),
		  .is_digital(1))
		Clock_1
		 (.clock_out(Net_748));


    ZeroTerminal ZeroTerminal_9 (
        .z(Net_741));

	wire [0:0] tmpOE__UART_Rx_net;
	wire [0:0] tmpIO_0__UART_Rx_net;
	wire [0:0] tmpINTERRUPT_0__UART_Rx_net;
	electrical [0:0] tmpSIOVREF__UART_Rx_net;

	cy_psoc3_pins_v1_10
		#(.id("1425177d-0d0e-4468-8bcc-e638e5509a9b"),
		  .drive_mode(3'b001),
		  .ibuf_enabled(1'b1),
		  .init_dr_st(1'b0),
		  .input_sync(1'b1),
		  .intr_mode(2'b00),
		  .io_voltage(""),
		  .layout_mode("CONTIGUOUS"),
		  .oe_conn(1'b0),
		  .output_conn(1'b0),
		  .output_sync(1'b0),
		  .pin_aliases(""),
		  .pin_mode("I"),
		  .por_state(4),
		  .use_annotation(1'b0),
		  .sio_group_cnt(0),
		  .sio_hyst(1'b0),
		  .sio_ibuf(""),
		  .sio_info(2'b00),
		  .sio_obuf(""),
		  .sio_refsel(""),
		  .sio_vtrip(""),
		  .slew_rate(1'b0),
		  .spanning(0),
		  .vtrip(2'b00),
		  .width(1))
		UART_Rx
		 (.oe(tmpOE__UART_Rx_net),
		  .y({1'b0}),
		  .fb({Net_717}),
		  .io({tmpIO_0__UART_Rx_net[0:0]}),
		  .siovref(tmpSIOVREF__UART_Rx_net),
		  .interrupt({tmpINTERRUPT_0__UART_Rx_net[0:0]}));

	assign tmpOE__UART_Rx_net = (`CYDEV_CHIP_MEMBER_USED == `CYDEV_CHIP_MEMBER_3A && `CYDEV_CHIP_REVISION_USED < `CYDEV_CHIP_REVISION_3A_ES3) ? ~{1'b1} : {1'b1};

    UART_v2_20_10 Pixy_UART (
        .cts_n(1'b0),
        .tx(Net_737),
        .rts_n(Net_738),
        .tx_en(Net_739),
        .clock(1'b0),
        .reset(Net_741),
        .rx(Net_717),
        .tx_interrupt(Net_742),
        .rx_interrupt(Net_743),
        .tx_data(Net_744),
        .tx_clk(Net_745),
        .rx_data(Net_746),
        .rx_clk(Net_747));
    defparam Pixy_UART.Address1 = 0;
    defparam Pixy_UART.Address2 = 0;
    defparam Pixy_UART.EnIntRXInterrupt = 0;
    defparam Pixy_UART.EnIntTXInterrupt = 0;
    defparam Pixy_UART.FlowControl = 0;
    defparam Pixy_UART.HalfDuplexEn = 0;
    defparam Pixy_UART.HwTXEnSignal = 1;
    defparam Pixy_UART.NumDataBits = 8;
    defparam Pixy_UART.NumStopBits = 1;
    defparam Pixy_UART.ParityType = 0;
    defparam Pixy_UART.RXEnable = 1;
    defparam Pixy_UART.TXEnable = 0;



endmodule

