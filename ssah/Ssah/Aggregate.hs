module Ssah.Aggregate where

import Data.List


--combine p (left:[]) rights = partition (p left) rights
combine' p (left:lefts) rights =
  ins:right
  where
    (ins, outs) = partition (p left) rights
    right = if (length lefts) > 0 then (combine' p lefts outs) else [outs]

-- does a join
combine p lefts rights =
  (init c, last c)
  where c = combine' p lefts rights

-- as combine, but barfs if there are remaining items
combineStrict p lefts rights =
  let (ins, outs) = combine p lefts rights in
  if (length outs) == 0 then ins else error "Application generated combine failure"

combineKeysStrict leftKey rightKey lefts rights =
  combineStrict p lefts rights
  where
    p l r = ((leftKey l) == (rightKey r))
    --p = p' l
                
testLefts = [10, 11, 12]
testRights =  [11, 12, 11, 12, 5, 11]
testAgg1 = combine (==)  testLefts testRights
-- ([[],[11,11,11],[12,12]],[5])

testAgg2 = combineStrict (==) testLefts testRights
