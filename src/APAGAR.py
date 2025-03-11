import matplotlib.pyplot as plt
import numpy as np

# Define tasks, durations, and start times (allowing parallel execution)
tasks = [
    "A", "B", 
    "C", "D",
    "E", "F",
    "G", "H", 
    "I"
]
durations = [2, 1, 3, 1, 1, 4, 0.1, 4, 3]  # Durations in weeks

# Define start weeks, allowing parallel execution
start_weeks = {
    "A": 0,
    "B": 2,
    "C": 3,
    "D": 3,
    "E": 4,
    "F": 5,
    "G": 9,
    "H": 9,
    "I": 13
}

# Define colors for each phase
colors = ['#2E86C1', '#27AE60', '#F39C12', '#8E44AD', '#3498DB', 
          '#E74C3C', '#1ABC9C', '#D35400', '#16A085']

# Create a waterfall chart
fig, ax = plt.subplots(figsize=(10, 5))
for i, task in enumerate(tasks):
    ax.barh(task, durations[i], left=start_weeks[task], color=colors[i], label=task if i < 10 else None)

# Customize labels and layout
ax.set_xlabel("Time")
ax.set_ylabel("Tasks")
ax.set_title("Experimental Procedure ")
ax.set_xticks(np.arange(0, max(start_weeks.values()) + max(durations) + 2, 2))
ax.grid(axis='x', linestyle='--', alpha=0.5)
plt.gca().invert_yaxis()  # Reverse order for readability

# Display the chart
plt.show()
