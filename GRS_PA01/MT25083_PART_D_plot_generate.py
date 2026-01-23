import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load the dataset
file_path = './MT25083_PART_D_CSV.csv'
df = pd.read_csv(file_path)

# Shorten names for better legibility in the graphs
# Transforms 'MT25083_Part_A_Program_A_cpu' -> 'A_cpu'
df['Label'] = df['Program+Function'].str.replace('MT25083_Part_A_Program_', '')

# List of factors we want to compare
factors = ['CPU%', 'Mem%', 'IO_Read(kB/s)', 'IO_Write(kB/s)', 'Time(s)']

# Set the visual style
sns.set_theme(style="whitegrid")

# Create a figure with subplots for each factor
fig, axes = plt.subplots(len(factors), 1, figsize=(12, 25), sharex=False)
fig.suptitle('Performance Factor Comparison: Program A vs Program B', fontsize=20, y=1.02)

for i, factor in enumerate(factors):
    sns.barplot(
        data=df,
        x='Workers',
        y=factor,
        hue='Label',
        ax=axes[i],
        palette='viridis'
    )
    axes[i].set_title(f'Comparison of {factor}', fontsize=15, fontweight='bold')
    axes[i].set_ylabel(factor)
    axes[i].legend(title='Program_Function', bbox_to_anchor=(1.05, 1), loc='upper left')

plt.tight_layout()

# Save the plot
plt.savefig('performance_comparison.png', bbox_inches='tight')
plt.show()