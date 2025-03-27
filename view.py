import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV data
data = pd.read_csv('output.csv')

print(data["magnetisation"])

betas = data['beta'].unique()
magnetisation = data['magnetisation'].values
energy_total = data['energy_total'].values

# Prepare the figure with subplots (2 rows and 2 columns)
fig, axs = plt.subplots(2, 2, figsize=(12, 10))

# 1. Histogram for Magnetisation with overlays for different betas
axs[0, 0].hist(magnetisation, bins=50, alpha=0.7, label="Magnetisation", color='blue')
axs[0, 0].set_title('Histogram of Magnetisation')
axs[0, 0].set_xlabel('Magnetisation')
axs[0, 0].set_ylabel('Frequency')

# Overlay histograms for different betas
for beta in betas:
    beta_data = data[data['beta'] == beta]
    axs[0, 0].hist(beta_data['magnetisation'], bins=50, alpha=0.5, label=f'beta={beta}')
    
axs[0, 0].legend()

# 2. Histogram for Energy with overlays for different betas
axs[0, 1].hist(energy_total, bins=50, alpha=0.7, label="Energy", color='red')
axs[0, 1].set_title('Histogram of Energy')
axs[0, 1].set_xlabel('Energy')
axs[0, 1].set_ylabel('Frequency')

# Overlay histograms for different betas
for beta in betas:
    beta_data = data[data['beta'] == beta]
    axs[0, 1].hist(beta_data['energy_total'], bins=50, alpha=0.5, label=f'beta={beta}')
    
axs[0, 1].legend()

# 3. Scatter plot of beta vs average magnetisation
avg_magnetisation = data.groupby('beta')['magnetisation'].mean()
axs[1, 0].scatter(avg_magnetisation.index, avg_magnetisation.values, color='green')
axs[1, 0].set_title('Beta vs Average Magnetisation')
axs[1, 0].set_xlabel('Beta')
axs[1, 0].set_ylabel('Average Magnetisation')

# 4. Scatter plot of beta vs average energy
avg_energy = data.groupby('beta')['energy_total'].mean()
axs[1, 1].scatter(avg_energy.index, avg_energy.values, color='purple')
axs[1, 1].set_title('Beta vs Average Energy')
axs[1, 1].set_xlabel('Beta')
axs[1, 1].set_ylabel('Average Energy')

# Adjust layout for better spacing
plt.tight_layout()

# Show the plots
plt.show()
