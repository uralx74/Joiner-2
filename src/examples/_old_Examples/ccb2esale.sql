select acct_id from (
  select acct_id 
    , row_number() over (order by null) N
  from ci_acct
) where N < 10 