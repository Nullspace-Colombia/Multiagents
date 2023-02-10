# main.py
import argparse
import struct
import numpy as np 

from env.bridge.TCP_IP_Connector import ServerHandler
from env.UE5_envs import CartPoleEnv

parser = argparse.ArgumentParser()

# Arguments definition 

parser.add_argument(
    "-s", "--server" ,
    action='store_true',
    help = "Init the server handler and bind a Socket Server"
    "to begin the TCP/IP communication."
)

parser.add_argument(
    "-c", "--client" ,
    action='store_true',
    help = "Begin the client service to connect to a Socket Server"
    "and start sending information."
)


if __name__ == "__main__":
    args = parser.parse_args()
    
    
    if args.client and args.server:
        raise ValueError("Client and Server flags are mutually exclusive. Please Allow only one of them.")

    """
        Client Service
    """ 
    if args.client:
        print("Creating a new client")
        env = CartPoleEnv()
        

        

        """
            First test for sending info 

            All actions send to the custom environment, have to be 
            send as an array || np array
        """
        sample_array_to_send = np.array([1.0, 1.0], dtype=np.single)
        action = sample_array_to_send
        # str_actions = np.array2string(actions)

        # value = bytes(str_actions, 'UTF-8')
        # ba  =  value # bytearray(struct.pack("f", value))  
        
        
        env.step(action)
        count = 1
        
        while True:
            try:
                if count % 10 == 0: 
                    response = env.step(action) # get environment response from applied action 
                    print("Step: {}".format(response)) #int.from_bytes(value, 'big', signed=True)))
                count += 1
            except KeyboardInterrupt:
                env.shutdown_server()
        
        
    
    elif args.server:      
        """
            Server Service
        """
        print("Creating a new server")
        handler = ServerHandler() 
        # Callback definition 
        def echo(msg):
            print(msg) 

        handler.set_callback(echo)
        handler.start() # init TCP/IP server 


