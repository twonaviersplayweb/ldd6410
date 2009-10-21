/*
 * ltt/probes/pm-trace.c
 *
 * Power Management tracepoint probes.
 */

#include <linux/module.h>
#include <trace/pm.h>

void probe_pm_idle_entry(void)
{
	trace_mark_tp(pm, idle_entry, pm_idle_entry,
		probe_pm_idle_entry, "irqstate #1%d",
		irqs_disabled());
}

void probe_pm_idle_exit(void)
{
	trace_mark_tp(pm, idle_exit, pm_idle_exit,
		probe_pm_idle_exit, "irqstate #1%d",
		irqs_disabled());
}

void probe_pm_suspend_entry(void)
{
	trace_mark_tp(pm, suspend_entry, pm_suspend_entry,
		probe_pm_suspend_entry, "irqstate #1%d",
		irqs_disabled());
}

void probe_pm_suspend_exit(void)
{
	trace_mark_tp(pm, suspend_exit, pm_suspend_exit,
		probe_pm_suspend_exit, "irqstate #1%d",
		irqs_disabled());
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mathieu Desnoyers");
MODULE_DESCRIPTION("Power Management Tracepoint Probes");
