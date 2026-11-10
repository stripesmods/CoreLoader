using UnityEngine;
using System.Collections.Generic;

namespace CoreLoader.UICommands
{
    public class UICommands : MonoBehaviour
    {
        // List of menu commands
        private Dictionary<string, System.Action> commands;

        void Awake()
        {
            commands = new Dictionary<string, System.Action>();

            // Example commands
            RegisterCommand("ToggleFly", ToggleFlyMode);
            RegisterCommand("ToggleSpeed", ToggleSpeedBoost);
            RegisterCommand("ShowMenu", ShowModMenu);
        }

        /// <summary>
        /// Registers a new UI command.
        /// </summary>
        public void RegisterCommand(string name, System.Action action)
        {
            if (!commands.ContainsKey(name))
            {
                commands.Add(name, action);
                Debug.Log($"[UICommands] Registered command: {name}");
            }
        }

        /// <summary>
        /// Executes a registered command.
        /// </summary>
        public void ExecuteCommand(string name)
        {
            if (commands.TryGetValue(name, out var action))
            {
                action.Invoke();
            }
            else
            {
                Debug.LogWarning($"[UICommands] Command not found: {name}");
            }
        }

        // Example command implementations
        private void ToggleFlyMode()
        {
            Debug.Log("[UICommands] Fly mode toggled.");
            // Add your custom logic here
        }

        private void ToggleSpeedBoost()
        {
            Debug.Log("[UICommands] Speed boost toggled.");
            // Add your custom logic here
        }

        private void ShowModMenu()
        {
            Debug.Log("[UICommands] Mod menu displayed.");
            // Add your custom logic here
        }
    }
}
