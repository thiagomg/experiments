(import [datetime [datetime timedelta]])

(defn show-age [name y m d]
  (setv cur (datetime.now))
  (setv dt (datetime y m d))
  (setv dty (datetime cur.year m d))
  (cond
    [(> dty cur) (setv dty (datetime (- cur.year 1) m d))])

  (print name)
  (print "    " (- cur dt))
  (print "    " (- cur.year y) "years" (- cur dty))
  (print "------------------------------"))

(show-age "Thiago" 1981 07 30)
(show-age "Adriana" 1980 9 7)
(show-age "Sofia" 2008 7 30)
(show-age "Julia" 2010 8 30)
(show-age "Vo Vera" 1928 12 14)
