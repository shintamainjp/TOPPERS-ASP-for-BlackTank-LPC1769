/*
 * タスクの情報取得機能
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "wait.h"
#include "semaphore.h"
#include "eventflag.h"
#include "dataqueue.h"
#include "pridataq.h"
#include "mailbox.h"
#include "mempfix.h"
#include "time_event.h"

/*
 *  タスクの情報取得機能
 */
#ifdef TOPPERS_inf_tsk

ER
inf_tsk(ID tskid, T_ITSK *pk_itsk)
{
    TCB *p_tcb;
    ER ercd = E_OK;
    uint_t tstat;

    p_tcb = get_tcb(tskid);
    t_lock_cpu();
    tstat = p_tcb->tstat;
    if (TSTAT_DORMANT(tstat)) {
        pk_itsk->stk_used = 0;
    } else {
        pk_itsk->stk_used = p_tcb->p_tinib->stksz
            - (p_tcb->tskctxb.sp - p_tcb->p_tinib->stk);
    }
    pk_itsk->stk_total = p_tcb->p_tinib->stksz;
    pk_itsk->tsk_pri_curr = EXT_TSKPRI(p_tcb->priority);
    pk_itsk->tsk_pri_base = EXT_TSKPRI(p_tcb->priority);
    pk_itsk->stk_head = p_tcb->p_tinib->stk;
    pk_itsk->stk_tail = p_tcb->p_tinib->stk + p_tcb->p_tinib->stksz - 1;
    t_unlock_cpu();

    return ercd;
}

#endif /* TOPPERS_inf_tsk */
