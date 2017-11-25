from datetime import datetime, timedelta

def show(name, y, m, d):
    cur = datetime.now()
    dt = datetime(y, m, d)
    dty = datetime(cur.year, m, d)
    if dty > cur:
        dty = datetime(cur.year-1, m, d)

    print( name )
    print( '   ', cur-dt )
    print( '   ', (cur.year-y), 'years', cur-dty )
    print( '------------------------------' )

show('Adriana', 1980, 9, 7)
show('Sofia', 2008, 7, 30)
show('Julia', 2010, 8, 30)
show('Vo Vera', 1928, 12, 14)
