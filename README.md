# Unray Bridge

Framework for communication between Unreal Engine and Python.

This repository contains all the files needed for Unreal Engine. 

## Unreal Engine Version

We are currently using Unreal Engine 5.1. We recommend using the same version to ensure project stability. 

## Project Files

In the Maps folder you'll find some examples to run:

![image](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/508e8fee-18fd-4caf-80e1-506919cfb012)

## Custom Envs

To create a custom env in Unreal Engine, first create your Agent Blueprint. 

You can create your agent based on the parent class of your choice. Once you create the blueprint, go to the ```Class Settings``` section.

![Class_Settings](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/10695530-d6b3-4b1e-a1a0-014e4a7b7008)

In the details panel, find the ```Interfaces``` section:

![Details_Panel](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/8ac78280-956a-4eff-9d62-5747af6514eb)

In the ```Implemented Interfaces``` subsection, click the ```Add``` button and search for "BI_Agent".

![Interface](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/ff7fb690-e23b-4c7a-9170-0fc0ff81d382)

![BI_Agent](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/ab14faee-521b-4f20-a7c4-c99736ea4ee9)

![Interface_Result](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/e0424945-374d-4786-8c1f-5db6ae402584)

Once you do this, in the blueprint functions, you'll now have these functions:

![Override_Functions](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/8709d437-53fb-4371-88bb-e4f8141d11a5)

You have to implement these functions according to your enviornment. 

| Function | Description |
|--------|---------|
|```Get Reward```| Agent Reward | 
| ```Is Done```| Function to specify the way the agent finishes the environment |
| ```Reset```  |  Reset the agent. ```Create Actor``` -> True if you want to destroy the actor and spawn it again in a nuew place. |
| ```Get State``` | Get agent observations |
| ```Step``` | What the agent does in each step |

When you've implemented all these functions and you want to try your environment, you'll have to add a **Connector** to your map.

In the Blueprints folder, you'll find the connectors for both single agent envs and multiagent envs:

![Connectors](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/f4fba381-5269-40c4-a5e6-7731922d9ca1)

### Single Agent Environments

If your environment is a single agent env, place a ```BP_Connector``` instance in your map. Once you do, you can select it and in the details panel you'll find the Default section. There, you'll find an Actor Agent variable, assign your agent to this variable. 

If you want to modify the IP and port, in the details panel you can input both parameters to your liking. The default values are: ``` IP = 127.0.0.1 ``` and ``` Port = 10010```

![Details_Connector](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/52acb043-5e69-4bbb-b164-3f38f6607908)



### MultiAgent Environments

If your environment is a multiagent env, you'll need to place a ```BP_Connector_MA``` instance in your map. Once you do, you can select it and in the details panel you'll find the Default section. There, you'll find an array called Actor Agents. 

![ConnectorMA_Panel](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/5989c256-024c-4386-b8c5-a585d493488f)

To ensure the framework can recognise all the agents in your environment, add each agent to the array. 

If you want to modify the IP and port, in the details panel you can input both parameters to your liking. The default values are: ``` IP = 127.0.0.1 ``` and ``` Port = 10011```

![Details_Connector_MA](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/0c17fb39-52d6-435e-8ff4-488a76d59db9)




