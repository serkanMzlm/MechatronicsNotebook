#!/usr/bin/python3

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier 

iris = load_iris()
# print(iris.keys())
# print(iris['DESCR']) Kısa bilgilendirme içerir
# print(iris['target_names'])
# print(iris['feature_names'])
# print(type(iris['data']))
#########################################

print("Data",iris['data'].shape) # Datanın boyutlarını gösterir.
print("_"*20)

# Veriler Test ve eğitim olarak iki gruba ayrılır
x_train, x_test, y_train, y_test = train_test_split(iris['data'], 
                                                    iris['target'], random_state=0)

print("X Train: ", x_train.shape)
print("Y Train: ", y_train.shape)
print("X Test: ", x_test.shape)
print("Y Test: ", y_test.shape)

# Data frame yapısına çevrilir.
iris_df = pd.DataFrame(x_train, columns=iris.feature_names)
print("_"*20)
print(iris_df.head())
print("_"*20)
# Saçılım matrix grafiği çizilir.
# pd.plotting.scatter_matrix(iris_df, c = y_train, figsize=(15, 15), marker='o',
#                         hist_kwds={'bins': 20}, s = 80, alpha = 0.8)
# plt.show()

knn = KNeighborsClassifier(n_neighbors=1)
knn.fit(x_train, y_train)

x_new = np.array([[7.5, 5, 7, 0.2]])
predict = knn.predict(x_new)

print("Predict Calss:", predict)
print("Predict Type:", iris['target_names'][predict])

# Tahinin Doğruluk oranı
y_predict = knn.predict(x_test)
print("Accuracy rate:", np.mean(y_predict == y_test))
# ya da 
print("Accuracy rate:", knn.score(x_test, y_test))