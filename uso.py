import serial
import threading
import tkinter as tk
import re

# ==========================
# シリアルポート設定
# ==========================
PORT = "COM7"      # Arduinoのポートに合わせて変更
BAUD = 9600

# ==========================
# グローバル変数
# ==========================
lie_score = 0
label_text = "🟢 正常"

# ==========================
# 判定ラベル
# ==========================
def get_label(score):
    if score >= 70:
        return "💥 嘘確定"
    elif score >= 40:
        return "⚠️ 疑わしい"
    elif score > 0:
        return "🟡 軽度"
    else:
        return "🟢 正常"

# ==========================
# シリアル通信
# ==========================
def serial_read():
    global lie_score, label_text

    ser = serial.Serial(PORT, BAUD, timeout=1)

    while True:
        try:
            line = ser.readline().decode("utf-8", errors="ignore").strip()
            print(line)

            match = re.search(r"Lie Score:\s*(\d+)", line)

            if match:
                score = int(match.group(1))
                lie_score = score
                label_text = get_label(score)

        except Exception as e:
            print(e)

# ==========================
# GUI更新
# ==========================
def update_gui():

    if lie_score >= 70:
        bg = "red"
    elif lie_score >= 40:
        bg = "orange"
    elif lie_score > 0:
        bg = "yellow"
    else:
        bg = "lightgreen"

    root.config(bg=bg)

    score_label.config(
        text=f"Lie Score : {lie_score}",
        bg=bg
    )

    status_label.config(
        text=label_text,
        bg=bg
    )

    root.after(100, update_gui)

# ==========================
# GUI作成
# ==========================
root = tk.Tk()
root.title("うそ発見器")
root.geometry("900x500")
root.configure(bg="lightgreen")

score_label = tk.Label(
    root,
    text="Lie Score : 0",
    font=("Arial", 60, "bold"),
    bg="lightgreen"
)
score_label.pack(pady=80)

status_label = tk.Label(
    root,
    text="🟢 正常",
    font=("Arial", 42, "bold"),
    bg="lightgreen"
)
status_label.pack()

# ==========================
# スレッド開始
# ==========================
threading.Thread(target=serial_read, daemon=True).start()

update_gui()
root.mainloop()