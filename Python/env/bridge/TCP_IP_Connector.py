""" 
    Server - server.py

    Server Class utilities definition to create a simple 
    TCP/IP communication. 
    =====================================================
    Classes 
    =====================================================
     - ClientHandler
     - ServerHandler
    ====================================================
    
    QUICK START GUIDE 
    =====================================================
        To check the 
    =====================================================
"""
import socket, sys
import numpy as np
import struct




class ClientHandler():
    __BUFFER_DATA_SIZE = 256
    """
    Client Handler 
    
    """
    def __init__(self): 
        self.connected = False 

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


        server_address = ('localhost', 10000)
        print('[ CONNECTION ] connecting to {} port {}'.format(*server_address))

        count = 1
        group_count = 1
        
        while not self.connected:
            try:
                self.sock.connect(server_address)
                self.connected = True 
            except: 
                count += 1
                if count % 10000 == 0: 
                    print("Trying to connect...")        
                    group_count += 1
                    if group_count % 5 == 0: 
                        
                        break 

        if not self.connected: 
            print("Connection Timeout!")                        
        else:
            print('[ CONNECTION ] Connected with server!')
            self.connected = False # Restart for future connections 
        

    def send(self, msg, __BUFFER_DATA_SIZE = 32):
        if not self.connected:
            assert "No server connection. Please check"
        print("[ SEND ]", end = " ")
        self.sock.sendall(msg)
        #print()

        print("[ RECV ]", end = " " )
        res = self.sock.recv(__BUFFER_DATA_SIZE) # Receiv the information from the environmente 
        observations = np.frombuffer(res, dtype=np.single)
        #position = struct.unpack('d', res)
        #print(res)
        print(observations)
        return res  # get response from server 

    def close(self): 
        print("Closing connection...")
        self.connected = False
        self.sock.close()
        print("Connection closed! Bye.")



class ServerHandler():
    
    """
    Server Handler 

    """
    def __init__(self, IP = 'localhost', PORT = 10000, max_connections = 2):
        self._IP   = IP
        self._PORT = PORT 
        self._MAX_CONNECTIONS = max_connections

        self.callback = None 
        self.number_connections = 0

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_address = (self._IP, self._PORT)

        self.sock.bind(self.server_address)
        self.sock.listen(10)
       
    def start(self, __BUFFER_DATA_SIZE = 256):
        print('[ SERVER ] Begining server on {}:{} | TCP/IP'.format(*self.server_address))
        while True:
            connection, client_address = self.sock.accept() # Wait for connections 
            # Connected Procedure 
            self.new_connection()
            print('[ NEW ] {}:{} connected! ({}, {})'.format(*client_address, self.number_connections, self._MAX_CONNECTIONS)) 

            try:
                while True:
                    data = connection.recv(__BUFFER_DATA_SIZE)
                    
                    # print('received {!r}'.format(data), end = " ")
                    #  print(f"converted to: {np.frombytes(data)}")
                    if data:
                        print(f'Action: {np.frombuffer(data, dtype = np.float16) * 2}')
                        
                        connection.sendall(data)
                    else:
                        print('no data from', client_address)
                        break
            except:
                print("[ WARN ] No callback config. Please use ServerHandler.set_callback(fcn)")


    def get_connections(self):
        return self.number_connections

    def new_connection(self):
        self.number_connections += 1

    def shutdown(self): 
        self.sock.close() 

    def set_callback(self, fcn = None): 
        self.callback = fcn 

    def test_connection(self): 
        pass 



