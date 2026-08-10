import os
import glob
import pandas as pd
import matplotlib.pyplot as plt

DATA_FOLDER = os.path.expanduser("~/data")
IMAGE_FOLDER = os.path.join(DATA_FOLDER, "image")
COLUMNS_TO_PLOT = ["VIO Pose X", "VIO Pose Y"]

def create_folders():
    os.makedirs(DATA_FOLDER, exist_ok=True)
    os.makedirs(IMAGE_FOLDER, exist_ok=True)
    print(f"Klasörler oluşturuldu: {DATA_FOLDER}, {IMAGE_FOLDER}")

def plot_single_graph(df, file_name):
    plt.figure(figsize=(10, 5))
    
    for col in COLUMNS_TO_PLOT:
        if col in df.columns:
            plt.plot(df[col], label=col, linestyle="-")

            min_index = df[col].idxmin()
            max_index = df[col].idxmax()
            min_value = df[col].min()
            max_value = df[col].max()

            plt.scatter(min_index, min_value, color="red", marker="o", s=50)
            plt.scatter(max_index, max_value, color="blue", marker="o", s=50)

            plt.annotate(f"Min: {min_value:.2f}", (min_index, min_value), textcoords="offset points", xytext=(-15,10), ha='center', fontsize=8, color="red")
            plt.annotate(f"Max: {max_value:.2f}", (max_index, max_value), textcoords="offset points", xytext=(15,-10), ha='center', fontsize=8, color="blue")

        else:
            print(f"{file_name} içinde '{col}' başlığı bulunamadı.")

    plt.title(f"{file_name}")
    plt.xlabel("Time")
    plt.ylabel("Distance (m)")
    plt.legend()
    plt.grid(True)

    output_path = os.path.join(IMAGE_FOLDER, f"{file_name}.png")
    plt.savefig(output_path, dpi=300)
    plt.show()
    plt.close()

    print(f"{output_path} kaydedildi.")

def plot_multiple_subplots(df, file_name):
    fig, axes = plt.subplots(1, len(COLUMNS_TO_PLOT), figsize=(15, 5))

    for ax, col in zip(axes, COLUMNS_TO_PLOT):
        if col in df.columns:
            ax.plot(df[col], label=col, linestyle="-")

            min_index = df[col].idxmin()
            max_index = df[col].idxmax()
            min_value = df[col].min()
            max_value = df[col].max()

            ax.scatter(min_index, min_value, color="red", marker="o", s=50)
            ax.scatter(max_index, max_value, color="blue", marker="o", s=50)

            ax.annotate(f"Min: {min_value:.2f}", (min_index, min_value), textcoords="offset points", xytext=(-15,10), ha='center', fontsize=8, color="red")
            ax.annotate(f"Max: {max_value:.2f}", (max_index, max_value), textcoords="offset points", xytext=(15,-10), ha='center', fontsize=8, color="blue")

            ax.set_title(col)
            ax.set_xlabel("Time")
            ax.set_ylabel("Distance (m)")
            ax.grid(True)
        else:
            ax.set_title(f"{col} (Veri yok)")
            ax.grid(True)

    plt.tight_layout()
    output_path = os.path.join(IMAGE_FOLDER, f"{file_name}.png")
    plt.savefig(output_path, dpi=300)
    plt.show()
    plt.close()

    print(f"{output_path} kaydedildi.")

def process_csv_files():
    csv_files = glob.glob(os.path.join(DATA_FOLDER, "*.csv")) 

    for csv_file in csv_files:
        try:
            df = pd.read_csv(csv_file)
            file_name = os.path.basename(csv_file).replace(".csv", "")
            plot_single_graph(df, file_name)  
            # plot_multiple_subplots(df, file_name)
        except Exception as e:
            print(f"{csv_file} işlenirken hata oluştu: {e}")

def main():
    create_folders()
    process_csv_files()

if __name__ == "__main__":
    main()