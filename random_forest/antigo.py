import numpy as np
import pandas as pd
import pydot
from sklearn.ensemble import RandomForestRegressor
from sklearn.model_selection import train_test_split
from sklearn.tree import export_graphviz


def main():
	dados = pd.read_csv('./random_forest/temps.csv', encoding='utf-8')
	#print(dados)
	dados_customizados = pd.get_dummies(dados)
	#print(features)
	dados_customizados.iloc[:,5:]

	# Labels are the values we want to predict
	labels = np.array(dados_customizados['actual'])

	# Remove the labels from the features
	# axis 1 refers to the columns
	dados_customizados= dados_customizados.drop('actual', axis = 1)

	# Saving feature names for later use
	feature_list = list(dados_customizados.columns)
	print(feature_list)
	# Convert to numpy array
	dados_customizados = np.array(dados_customizados)

	people = dados['month'].value_counts()
	#print(people)

	# Split the data into training and testing sets
	train_features, test_features, train_labels, test_labels = train_test_split(dados_customizados, labels, test_size = 0.25, random_state = 42)
	print('Training Features Shape:', train_features.shape)
	print('Training Labels Shape:', train_labels.shape)
	print('Testing Features Shape:', test_features.shape)
	print('Testing Labels Shape:', test_labels.shape)

	baseline_preds = test_features[:, feature_list.index('average')]
	# Baseline errors, and display average baseline error
	baseline_errors = abs(baseline_preds - test_labels)
	print('Average baseline error: ', round(np.mean(baseline_errors), 2))

	# Instantiate model with 1000 decision trees
	random_forest = RandomForestRegressor(n_estimators = 1000, random_state = 42)
	# Train the model on training data
	random_forest.fit(train_features, train_labels);

	previsoes = random_forest.predict(test_features)
	# Calculate the absolute errors
	erros = abs(previsoes - test_labels)
	# Print out the mean absolute error (mae)
	print('Mean Absolute Error:', round(np.mean(erros), 2), 'degrees.')

	# Calculate mean absolute percentage error (MAPE)
	mape = 100 * (errors / test_labels)
	# Calculate and display accuracy
	accuracy = 100 - np.mean(mape)
	print('Accuracy:', round(accuracy, 2), '%.')


	# Pull out one tree from the forest
	tree = rf.estimators_[5]
	# Import tools needed for visualization
	from sklearn.tree import export_graphviz
	import pydot
	# Pull out one tree from the forest
	tree = rf.estimators_[5]
	# Export the image to a dot file
	export_graphviz(tree, out_file = 'tree.dot', feature_names = feature_list, rounded = True, precision = 1)
	# Use dot file to create a graph
	(graph, ) = pydot.graph_from_dot_file('tree.dot')
	# Write graph to a png file
	graph.write_png('tree.png')

	# Limit depth of tree to 3 levels
	rf_small = RandomForestRegressor(n_estimators=10, max_depth = 3)
	rf_small.fit(train_features, train_labels)
	# Extract the small tree
	tree_small = rf_small.estimators_[5]
	# Save the tree as a png image
	export_graphviz(tree_small, out_file = 'small_tree.dot', feature_names = feature_list, rounded = True, precision = 1)
	(graph, ) = pydot.graph_from_dot_file('small_tree.dot')
	graph.write_png('small_tree.png');



if __name__ == "__main__":
	main()