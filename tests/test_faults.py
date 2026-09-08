from host.simulator import NORMAL, WARNING, CRITICAL, classify_fault, VirtualMCU


def test_nominal_is_normal():
    assert classify_fault(3.30, 0.50, 35.0) == NORMAL


def test_warning_threshold():
    assert classify_fault(3.55, 0.50, 35.0) == WARNING


def test_overvoltage_is_critical():
    assert classify_fault(3.80, 0.50, 35.0) == CRITICAL


def test_undervoltage_is_critical():
    assert classify_fault(2.50, 0.50, 35.0) == CRITICAL


def test_overcurrent_is_critical():
    assert classify_fault(3.30, 2.50, 35.0) == CRITICAL


def test_watchdog_detects_missed_ticks():
    mcu = VirtualMCU()
    assert mcu.missed_task_ticks(4) is True
