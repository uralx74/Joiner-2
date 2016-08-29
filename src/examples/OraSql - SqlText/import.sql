select 
  trunc(CHAR_VAL_DTTM,'mm') CHAR_VAL_DTTM
  , '' test_field 
from CFG_TASK where task_name like 'P_UPDATE_NASEL_JOB_DAY'