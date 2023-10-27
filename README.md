# Unray Bridge

Framework for communication between Unreal Engine and Python.

This repository contains all the files needed for Unreal Engine. 

## Unreal Engine Version

We are currently using Unreal Engine 5.1. We recommend using the same version to ensure project stability. 

## Project Files

In the Maps folder you'll find some examples to run:

![Maps](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/dd5a8093-a6da-4c88-8731-1b993d9faec8)


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
| ```Reset```  |  Reset the agent. ```Create Actor``` -> True if you want to destroy the actor and spawn it again in a new place. |
| ```Get State``` | Get agent observations |
| ```Step``` | What the agent does in each step |

When you've implemented all these functions and you want to try your environment, you'll have to add a **Connector** to your map.

In the Blueprints folder, you'll find the connectors for both single agent envs and multiagent envs:

![Connectors](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/d7e4c7f1-f234-4b0d-88bd-e2d2fcc188b4)

### Single Agent Environments

If your environment is a single agent env, place a ```Connector_SA``` instance in your map. Once you do, you can select it and in the details panel you'll find the Default section. There, you'll find an Actor Agent variable, assign your agent to this variable. 

### MultiAgent Environments

If your environment is a multiagent env, you'll need to place a ```Connector_MA``` instance in your map. Once you do, you can select it and in the details panel you'll find the Default section. There, you'll find an array called Actor Agents. 

![ConnectorMA_Panel](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/5989c256-024c-4386-b8c5-a585d493488f)

To ensure the framework can recognise all the agents in your environment, add each agent to the array. 

Remember that for each agent in your env, you'll have to implement the Reward, Done, Reset, Get State and Step functions.

## Parallel Trainning

If you want to train several envs at the same time, we recommend you create your env as a Blueprint. 

In the Blueprints folder you'll find a ```MultiAgent_Env``` Blueprint.

![Env](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/36be5b8e-78bb-4144-8882-b9e6b8d154e1)

You can create your env with this Blueprint as a parent Class.

In the Viewport of your env blueprint class, drag the connector you need from the Content Drawer and place it where you want. 

In the Event Graph of your env blueprint class, you'll have to do a few things to configure your env. 

First, each env you create will have an ID (which defaults to 1). You can either set this parameter in the Details pannel of your map or create a function to set it automatically.

Then, you need to add the agents in your env to an ```Agents``` array, which belongs to the ```MultiAgent_Env``` class. To do so, simply search for the ```Get Agents``` function and add each of your agents to this array. For example, in the MultiAgent Arena map it looks like this:

![AddAgents](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/7f6670bc-7bc0-4163-b854-652bcefdb9b3)

Finally, you'll have to add the following functions to your env class:

![MultiAgentEnv](https://github.com/Nullspace-Colombia/Multiagents/assets/55969494/ba178e2f-6dc9-4fd0-9d7a-61f71d9e06ef)

This is to set the agents and set the ports in which the communication is going to happen.








