select 
  ci_acct.acct_id
  , decode (pp.acct_id, null, 0, 1) pp
  , decode (fa.acct_id, null, 0, 1) fa
  , to_date('/**:dt**/','yyyy-mm-dd') dt
  , /**:test_field**/ test_field

from ci_acct
