""" 
    UE5_envs.py
"""
import gym.spaces as spaces
import numpy as np

from .bridge.TCP_IP_Connector import ClientHandler
from gym import Env as gymEnv


class CartPoleEnv(gymEnv):
    """
    Observation space 
    ============================
    |  1  |    cart position   |
    |  2  |    cart velocity   |
    |  3  |    pole position   |
    |  4  |    pole velocity   |
    ============================

    From: https://www.gymlibrary.dev/environments/classic_control/cart_pole/

    """
    metadata = { 
        "render_modes": ["human", "rgb_array"], 
        "render_fps": 4
    } # Gym Meta Data

    def __init__(self, render_mode = None): 
        
        # Simulation parameters 
        self.__CART_POS_THRESH = 2.4
        self.__POLE_POS_THRESH = 12
        self.__MAX_FRAMES =  500 # For v0


        # Spaces definition 
        high = np.array(
            [
                self.__CART_POS_THRESH * 2,
                np.finfo(np.float32).max,
                self.__POLE_POS_THRESH * 2,
                np.finfo(np.float32).max,
            ],
            dtype=np.float32,
        )

        self.action_space = spaces.Discrete(2)
        self.observation_space = spaces.Box(-high, high, dtype=np.float32)

        self.render_mode = None 
        
        self.counter = 0

     
        self.handler = ClientHandler()

    def step(self, action):

        if isinstance(action, list): 
            action = np.array(action)
        elif isinstance(action, np.ndarray):
            pass
        else:
            assert "No valid action type. Only supports <list> or <numpy.ndarray> given %s" % (type(action))
        
        # process the action from np array to byte buffer 
        action_buff = action.tobytes()


        # Send to the server and wait for response 
        res = self.handler.send(action_buff) # Send action an wait response 
        obs = [0,0,0,0]
        
        # Rewards System 
        self.counter += 1 
        reward = self.counter

        # Termination conditions 
        try: 
            cartPos, cartVel, polePos, poleVel = obs
        except: 
            cartPos, cartVel, polePos, poleVel = [0, 0, 0, 0]
        
        if abs(cartPos) >= self.__CART_POS_THRESH or abs(polePos) >= self.__POLE_POS_THRESH or self.counter >= self.__MAX_FRAMES:
            terminated = True
        else: 
            terminated = False
        
        # Additional data 
        truncated = False
        info = {}

        return obs, reward, terminated, truncated, info

    def resp2obs(self, resp):
        """
            resp2obs 
            - Convert the socket response into a valid observation vector state 

            @args resp - Socket server response 
            @returns obs - parse observation vector 

        """
        # obs = int.from_bytes(resp, byteorder='big', signed=True)
        # dt = np.dtype("floa")
        # dt = dt.newbyteorder('<')
        # print(resp)
        obs = np.frombuffer(resp, count = -1, dtype= np.float16) #, dtype= dt)
            
        return obs

    def reset(self):
        print("Reseting!")

    def shutdown_server(self):
        self.handler.close()