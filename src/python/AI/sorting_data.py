#!/usr/bin/python3
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from sklearn.manifold import Isomap
from sklearn.decomposition import PCA
from sklearn.datasets import load_digits
from sklearn.metrics import accuracy_score
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import confusion_matrix
from sklearn.model_selection import train_test_split

digits = load_digits()
print(digits.images.shape)

fig, axes = plt.subplots(10, 10, figsize = (8, 8), subplot_kw = { 'xticks': [], 'yticks': []}, 
                         gridspec_kw=dict (hspace=0.1, wspace=0.1))
for i, ax in enumerate (axes.flat):
    ax.imshow(digits.images[i], cmap='binary', interpolation='nearest')
    ax.text(0.05, 0.05, str(digits.target[i]), transform = ax.transAxes, color='green')
plt.show()

x = digits.data
y = digits.target
print("x:", x.shape, "| y", y.shape)

iso = Isomap(n_components=2, n_neighbors=5)
iso.fit(x)
data2D = iso.transform(x)
# print(data2D.shape)

plt.scatter(data2D[:, 0], data2D[:, 1], c=digits.target, alpha=0.5, cmap = plt.cm.get_cmap('tab10', 10))
plt.colorbar(label = "Digit", ticks = range(10))
plt.show()

x_train, x_test, y_train, y_test = train_test_split(x, y, random_state=0)
model = GaussianNB()
model.fit(x_train, y_train)
y_model = model.predict(x_test)

print("Accuracy Rate",accuracy_score(y_test, y_model))
mat = confusion_matrix(y_test, y_model)
sns.heatmap(mat, square = True, annot = True, cbar = False)

plt.xlabel('Real Value')
plt.ylabel('Estimated Value')

plt.show()