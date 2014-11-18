#lang racket  ;-*-Scheme-*-

(provide *seconds-in-day* *seconds-in-year* epoch epoch->string epoch->years epoch-today
         find-years today years-today)

(require racket/date)
(require carali/formatting)
  
(define *seconds-in-day*
  ; number of seconds in a day
  (* 60 60 24))

 
(define *seconds-in-year* (* *seconds-in-day* 365.25))

(define (scale-ymd  y m d by)
  (/ (find-seconds 0 0 0 d m y) by))

(define (apply-to-today func)
  (let ((d (current-date)))
    (func (date-year d) (date-month d) (date-day d))))
  
(define (find-years y m d)
  ;; convert year-month-day  into seconds since epoch
  (scale-ymd y m d *seconds-in-year*))
 
(define (years-today)
  (apply-to-today find-years))
  

(define (epoch y m d)
  (scale-ymd y m d *seconds-in-day*))
  
       
  
(define (epoch-today) (apply-to-today epoch))
;(epoch-today)

(define (epoch->string e)
  (date-display-format 'iso-8601)
  (date->string (seconds->date (* e *seconds-in-day*))))
 
(define (epoch->years e) (/ e 365.25))

(define today current-date)

(provide ymd->epoch)
(define (ymd->epoch s)
  (define (part a b) (string->number (substring s a b)))
  (define y (part 0 4))
  (define m (part 5 7))
  (define d (part 8 10))
  ;(formatln "~A ~a ~a" y m d)
  (epoch y m d))
;(ymd->epoch "2011-10-31") => 15278


(provide ymd->years)
(define (ymd->years s)
  ;convert a string to a year figure
  (epoch->years (ymd->epoch s)))