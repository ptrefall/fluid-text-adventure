![Fluid Hierarchical Task Network](https://i.imgur.com/xKfIV0f.png)
# Fluid Text Adventure
A simple text adventure example that takes advantage of the planning capabilities of the [Fluid Hierarchical Task Network](https://github.com/ptrefall/fluid-hierarchical-task-network) planner. We also use the Random Selector in the [Fluid Extensions](https://github.com/ptrefall/fluid-hierarchical-task-network-ext) library.

The example sets up a scenario with a bottle and a sword in an otherwise empty room. You can pick up the bottle and sword, you can open the bottle and drink from it, throw the bottle or break it with the sword. The planner will be able to string together actions in order to fulfill the player's command conveniently. So that if you type 'drink bottle' right from the start, the planner will know to first pick up the bottle, unscrew the cork, and then drink it, for example.

This is just a simple example that show 'a' way to use the planner. The way objects and the planner domains are set up are in no way perfect or generic enough, but hopefully it can help bring insight into the use of the planner.

This example will improve over time.

## Support
Join the [discord channel](https://discord.gg/MuccnAz) to share your experience and get support on the usage of Fluid HTN.
