import socket, threading, pymysql,time


def sqlIn(li):
    sqlInsert = "insert into initial_data(addr, weight, ref_weight, time_stamp,sign) values(%s,%s,%s,%s,%s)"
    try:
        cursor.execute(sqlInsert, li)
        print('成功添加'+cursor.rowcount+'行')
    except Exception as e:
        print("Reason:", e)
        db.rollback()


def tcplink(sock, addr):
    print('Accept new connection from %s:%s...' % addr)
    sign = time.time()
    sign = repr(sign)
    while True:
        st = sock.recv(1024*10)
        if not st:
            break
        st = st.decode('utf-8')
        print(st)
        st = str(st)
        li = st.split(':')

        if li[3] == 0 or li[3] == 00 or li[3] == '00' or li[3] == '0':
            sign = time.time()
            sign = repr(sign)
            continue

        li.append(sign)
        sqlIn(li)
    sock.close()


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('10.2.130.178', 9000))
s.listen(10)
print('Waiting for connection...')

# 连接数据库
db = pymysql.connect('localhost', port=3306, user='root', passwd='960921', db='weight_data')
cursor = db.cursor()

while True:
    sock, addr = s.accept()
    t = threading.Thread(target=tcplink, args=(sock, addr))
    t.start()

db.close()