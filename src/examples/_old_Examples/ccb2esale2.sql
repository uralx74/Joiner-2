select acct_id from (
  select '6669996666' acct_id 
    , row_number() over (order by null) N
  from ci_acct
) where N < 10 