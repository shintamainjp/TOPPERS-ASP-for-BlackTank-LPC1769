{
	"TimeScale" :"us",
	"TimeRadix" :10,
	"ConvertRules"   :["asp"],
	"VisualizeRules" :["toppers","asp"],
	"ResourceHeaders":["asp"],
	"Resources":
	{
		"CurrentContext":{
			"Type":"Context",
			"Attributes":
			{
				"name"    : "None"
			}
		},
		"LOGTASK":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :1,
				"atr"   :"TA_ACT",
				"pri"   :3,
				"exinf" :"LOGTASK_PORTID",
				"task"  :"logtask_main",
				"stksz" :1024,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK_INIT":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :2,
				"atr"   :"TA_ACT",
				"pri"   :13,
				"exinf" :"0",
				"task"  :"task_init",
				"stksz" :2048,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK_DISPLAY":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :3,
				"atr"   :"0",
				"pri"   :11,
				"exinf" :"0",
				"task"  :"task_display",
				"stksz" :2048,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"TASK_LED":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :4,
				"atr"   :"0",
				"pri"   :11,
				"exinf" :"0",
				"task"  :"task_led",
				"stksz" :2048,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"TASK_NTSHELL":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :5,
				"atr"   :"0",
				"pri"   :14,
				"exinf" :"0",
				"task"  :"task_ntshell",
				"stksz" :2048,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"TASK_AUDIO":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :6,
				"atr"   :"0",
				"pri"   :10,
				"exinf" :"0",
				"task"  :"task_audio",
				"stksz" :2048,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"TASK_MENU":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :7,
				"atr"   :"0",
				"pri"   :12,
				"exinf" :"0",
				"task"  :"task_menu",
				"stksz" :2048,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"TASK_USERINPUT":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :8,
				"atr"   :"0",
				"pri"   :12,
				"exinf" :"0",
				"task"  :"task_userinput",
				"stksz" :2048,
				"stk"   :"NULL",
				"state" :"DORMANT"
			}
		},
		"INH_INHNO_TIMER_target_timer_handler":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"id"    :15,
				"state"    : "DORMANT"
			}
		},
		"INH_21__kernel_inthdr_21":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"id"    :21,
				"state"    : "DORMANT"
			}
		},
		"INH_42__kernel_inthdr_42":{
			"Type":"InterruptHandler",
			"Attributes":
			{
				"id"    :42,
				"state"    : "DORMANT"
			}
		},
		"ISR_INTNO_SIO_sio_isr_SIO_PORTID":{
			"Type":"InterruptServiceRoutine",
			"Attributes":
			{
				"id"    :21,
				"state"    : "DORMANT"
			}
		},
		"ISR_INTNO_DMA_dma_intr_handler_0":{
			"Type":"InterruptServiceRoutine",
			"Attributes":
			{
				"id"    :42,
				"state"    : "DORMANT"
			}
		},
,
,
,
		"TASK_TEX":{
			"Type":"TaskExceptionRoutine",
			"Attributes":
			{
				"state"    : "DORMANT"
			}
		}
	}
}

