#lang racket

(require carali/datetime)
(require rackunit)

(check-eq? (- (epoch 2010 7 1) (epoch 2010 6 30)) 
           1 
           "1 Jul 2010 is one day after 30 Jun 2010")