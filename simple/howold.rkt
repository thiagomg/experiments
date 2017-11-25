#lang racket

(require gregor)

(define (diff-dates f s)
  (- (->jdn s) (->jdn f)))

(define (how-old-days bday cur-date)
  (diff-dates bday cur-date))

(define (get-last-bday bday cur-date)
  (let ([last-bday
          (date (->year cur-date) (->month bday) (->day bday))])
    (if (date>? last-bday cur-date)
        (date (- (->year cur-date) 1) (->month bday) (->day bday))
        last-bday))
)

(define (how-old-year bday last-bday cur-date)
  (list
     (- (->year last-bday) (->year bday))
     (diff-dates last-bday cur-date)
     )
  )

(define (show-age name y m d)
  (let* (
         [bday (date y m d)]
         [cur-date (->date (now))]
         [last-bday (get-last-bday bday cur-date)]
         [num-days (how-old-days bday cur-date)]
         [year-and-day (how-old-year bday last-bday cur-date)]
         )
    (begin
      (displayln name)
      (printf "    ~a days\n" num-days)
      (printf "    ~a years ~a days\n" (car year-and-day) (cadr year-and-day))
      (displayln "------------------------------")
      )
    ))

(begin
  (show-age "Thiago" 1981 07 30)
  (show-age "Adriana" 1980 9 7)
  (show-age "Sofia" 2008 7 30)
  (show-age "Julia" 2010 8 30)
  (show-age "Vo Vera" 1928 12 14)
)
