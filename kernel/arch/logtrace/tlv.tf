$FILE "kernel.res"$
{$NL$
$TAB$"TimeScale" :"us",$NL$
$TAB$"TimeRadix" :10,$NL$
$TAB$"ConvertRules"   :["asp"],$NL$
$TAB$"VisualizeRules" :["toppers","asp"],$NL$
$TAB$"ResourceHeaders":["asp"],$NL$
$TAB$"Resources":$NL$
$TAB${$NL$
$TAB$$TAB$"CurrentContext":{$NL$
$TAB$$TAB$$TAB$"Type":"Context",$NL$
$TAB$$TAB$$TAB$"Attributes":$NL$
$TAB$$TAB$$TAB${$NL$
$TAB$$TAB$$TAB$$TAB$"name"    : "None"$NL$
$TAB$$TAB$$TAB$}$NL$
$TAB$$TAB$},$NL$
$JOINEACH tskid TSK.ID_LIST ",\n"$
    $TAB$$TAB$"$tskid$":{$NL$
    $TAB$$TAB$$TAB$"Type":"Task",$NL$
    $TAB$$TAB$$TAB$"Attributes":$NL$
    $TAB$$TAB$$TAB${$NL$
    $TAB$$TAB$$TAB$$TAB$"id"    :$+tskid$,$NL$
    $TAB$$TAB$$TAB$$TAB$"atr"   :"$TSK.TSKATR[tskid]$",$NL$
    $TAB$$TAB$$TAB$$TAB$"pri"   :$+TSK.ITSKPRI[tskid]$,$NL$
    $TAB$$TAB$$TAB$$TAB$"exinf" :"$TSK.EXINF[tskid]$",$NL$
    $TAB$$TAB$$TAB$$TAB$"task"  :"$TSK.TASK[tskid]$",$NL$
    $TAB$$TAB$$TAB$$TAB$"stksz" :$+TSK.STKSZ[tskid]$,$NL$
    $TAB$$TAB$$TAB$$TAB$"stk"   :"NULL",$NL$
    $TAB$$TAB$$TAB$$TAB$"state" :
        $IF TSK.TSKATR[tskid] == TA_ACT$
            "RUNNABLE"$NL$
        $ELSE$
            "DORMANT"$NL$
        $END$
    $TAB$$TAB$$TAB$}$NL$
    $TAB$$TAB$}
$END$,
$NL$
$JOINEACH inhno INH.ORDER_LIST ",\n"$
    $TAB$$TAB$"INH_$INH.INHNO[inhno]$_$INH.INTHDR[inhno]$":{$NL$
    $TAB$$TAB$$TAB$"Type":"InterruptHandler",$NL$
    $TAB$$TAB$$TAB$"Attributes":$NL$
    $TAB$$TAB$$TAB${$NL$
    $TAB$$TAB$$TAB$$TAB$"id"    :$+INH.INHNO[inhno]$,$NL$
    $TAB$$TAB$$TAB$$TAB$"state"    : "DORMANT"$NL$
    $TAB$$TAB$$TAB$}$NL$
    $TAB$$TAB$}
$END$,
$NL$
$JOINEACH order ISR.ORDER_LIST ",\n"$
    $TAB$$TAB$"ISR_$ISR.INTNO[order]$_$ISR.ISR[order]$_$ISR.EXINF[order]$":{$NL$
    $TAB$$TAB$$TAB$"Type":"InterruptServiceRoutine",$NL$
    $TAB$$TAB$$TAB$"Attributes":$NL$
    $TAB$$TAB$$TAB${$NL$
    $TAB$$TAB$$TAB$$TAB$"id"    :$+ISR.INTNO[order]$,$NL$
    $TAB$$TAB$$TAB$$TAB$"state"    : "DORMANT"$NL$
    $TAB$$TAB$$TAB$}$NL$
    $TAB$$TAB$}
$END$,
$NL$
$JOINEACH cycid CYC.ID_LIST ",\n"$
    $TAB$$TAB$"$cycid$":{$NL$
    $TAB$$TAB$$TAB$"Type":"CyclicHandler",$NL$
    $TAB$$TAB$$TAB$"Attributes":$NL$
    $TAB$$TAB$$TAB${$NL$
    $TAB$$TAB$$TAB$$TAB$"id"    :$+cycid$,$NL$
    $TAB$$TAB$$TAB$$TAB$"state"    : "DORMANT"$NL$
    $TAB$$TAB$$TAB$}$NL$
    $TAB$$TAB$}
$END$,
$NL$
$JOINEACH almid ALM.ID_LIST ",\n"$
    $TAB$$TAB$"$almid$":{$NL$
    $TAB$$TAB$$TAB$"Type":"AlarmHandler",$NL$
    $TAB$$TAB$$TAB$"Attributes":$NL$
    $TAB$$TAB$$TAB${$NL$
    $TAB$$TAB$$TAB$$TAB$"id"    :$+almid$,$NL$
    $TAB$$TAB$$TAB$$TAB$"state"    : "DORMANT"$NL$
    $TAB$$TAB$$TAB$}$NL$
    $TAB$$TAB$}
$END$,
$NL$
$JOINEACH excno EXC.ORDER_LIST ",\n"$
    $TAB$$TAB$"$EXC.EXCNO[excno]$":{$NL$
    $TAB$$TAB$$TAB$"Type":"CPUExceptionHandler",$NL$
    $TAB$$TAB$$TAB$"Attributes":$NL$
    $TAB$$TAB$$TAB${$NL$
    $TAB$$TAB$$TAB$$TAB$"id"    :$+EXC.EXCNO[excno]$,$NL$
    $TAB$$TAB$$TAB$$TAB$"state"    : "DORMANT"$NL$
    $TAB$$TAB$$TAB$}$NL$
    $TAB$$TAB$}
$END$,
$NL$
    $TAB$$TAB$"TASK_TEX":{$NL$
    $TAB$$TAB$$TAB$"Type":"TaskExceptionRoutine",$NL$
    $TAB$$TAB$$TAB$"Attributes":$NL$
    $TAB$$TAB$$TAB${$NL$
    $TAB$$TAB$$TAB$$TAB$"state"    : "DORMANT"$NL$
    $TAB$$TAB$$TAB$}$NL$
    $TAB$$TAB$}
$NL$
$TAB$}$NL$
}$NL$
$FILE "kernel_cfg.c"$
