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
		"TASK_MENU":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :2,
				"atr"   :"TA_ACT",
				"pri"   :11,
				"exinf" :"0",
				"task"  :"task_menu",
				"stksz" :1024,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK_DISPLAY":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :3,
				"atr"   :"TA_ACT",
				"pri"   :10,
				"exinf" :"0",
				"task"  :"task_display",
				"stksz" :1024,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK_USERINPUT":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :4,
				"atr"   :"TA_ACT",
				"pri"   :12,
				"exinf" :"0",
				"task"  :"task_userinput",
				"stksz" :1024,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK_LED":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :5,
				"atr"   :"TA_ACT",
				"pri"   :10,
				"exinf" :"0",
				"task"  :"task_led",
				"stksz" :1024,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK_NTSHELL":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :6,
				"atr"   :"TA_ACT",
				"pri"   :12,
				"exinf" :"0",
				"task"  :"task_ntshell",
				"stksz" :1024,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK_AUDIO_INIT":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :7,
				"atr"   :"TA_ACT",
				"pri"   :16,
				"exinf" :"0",
				"task"  :"task_audio_init",
				"stksz" :1024,
				"stk"   :"NULL",
				"state" :"RUNNABLE"
			}
		},
		"TASK_AUDIO":{
			"Type":"Task",
			"Attributes":
			{
				"id"    :8,
				"atr"   :"0",
				"pri"   :9,
				"exinf" :"0",
				"task"  :"task_audio",
				"stksz" :1024,
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

