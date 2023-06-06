import pandas as pd
import ROOT

# Открытие ROOT файла
file = ROOT.TFile("check.root", "read")

# Чтение дерева из файла
tree = file.Get("Data")

# Создание массива NumPy из данных дерева
data = []

for event in tree:
    # Извлечение значений переменных из дерева
    x = event.x
    y = event.Energy
    
    # Добавление значений в массив
    data.append([x, y])

# Создание DataFrame с помощью pandas
df = pd.DataFrame(data, columns=["x", "y"])

# Сохранение DataFrame в CSV файл
df.to_csv("file.csv", index=False)