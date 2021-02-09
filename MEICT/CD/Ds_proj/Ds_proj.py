####################################################
############### Data Science project ###############
####################################################
################	Group 30		################
##	Pedro Alexandre Lopes Correia de Campos - 83951
##	Tomás da Silva Faria Lampreia Gomes		- 90782
##	Pedro de Oliveira Rosa Alves Leitão		- 90764

import os
import pandas as pd
from pandas.plotting import register_matplotlib_converters
import matplotlib.pyplot as plt
import ds_functions as ds
import seaborn as sns
import scipy.stats as _stats
import numpy as np
from sklearn.impute import SimpleImputer
from sklearn.preprocessing import StandardScaler, MinMaxScaler
from sklearn.model_selection import train_test_split
import sklearn.metrics as metrics
from sklearn.naive_bayes import GaussianNB, MultinomialNB, BernoulliNB
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from subprocess import call
from sklearn.tree import export_graphviz
from sklearn.ensemble import RandomForestClassifier
from imblearn.over_sampling import SMOTE
import sklearn.metrics as metrics
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from itertools import combinations
from sklearn.base import clone
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
from sklearn.mixture import GaussianMixture
from sklearn.cluster import DBSCAN
from scipy.spatial.distance import pdist, squareform
from sklearn.cluster import AgglomerativeClustering
from sklearn.decomposition import PCA
import mlxtend.frequent_patterns as pm
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import KFold
from sklearn.model_selection import LeaveOneOut

###### Auxiliar functions ###########
#Function to calculate the number of different values in a list
def different(values: list):
    different: List = []
    for i in values:
        if different.count(i) == 0:
            different.append(i)
    return len(different)

def getBins(dif: int):
    num = dif
    if dif > 10:
        num = 10
        if dif > 100:
            num = 100
            if dif > 1000:
                num = 1000
                if dif > 10000:
                    num = 10000
    return num

def plot_top_rules(rules: pd.DataFrame, metric: str, per_metric: str) -> None:
    _, ax = plt.subplots(figsize=(6, 3))
    ax.grid(False)
    ax.set_axis_off()
    ax.set_title(f'TOP 10 per Min {per_metric} - {metric}', fontweight="bold")
    text = ''
    cols = ['antecedents', 'consequents']
    rules[cols] = rules[cols].applymap(lambda x: tuple(x))
    for i in range(len(rules)):
        rule = rules.iloc[i]
        text += f"{rule['antecedents']} ==> {rule['consequents']}"
        text += f"(s: {rule['support']:.2f}, c: {rule['confidence']:.2f}, lift: {rule['lift']:.2f})\n"
    ax.text(0, 0, text)
    plt.show()

def analyse_per_metric(rules: pd.DataFrame, metric: str, metric_values: list) -> list:
    print(f'Analyse per {metric}...')
    conf = {'avg': [], 'top25%': [], 'top10': []}
    lift = {'avg': [], 'top25%': [], 'top10': []}
    top_conf = []
    top_lift = []
    nr_rules = []
    for m in metric_values:
        rs = rules[rules[metric] >= m]
        nr_rules.append(len(rs))
        conf['avg'].append(rs['confidence'].mean(axis=0))
        lift['avg'].append(rs['lift'].mean(axis=0))

        top_conf = rs.nlargest(int(0.25*len(rs)), 'confidence')
        conf['top25%'].append(top_conf['confidence'].mean(axis=0))
        top_lift = rs.nlargest(int(0.25*len(rs)), 'lift')
        lift['top25%'].append(top_lift['lift'].mean(axis=0))

        top_conf = rs.nlargest(10, 'confidence')
        conf['top10'].append(top_conf['confidence'].mean(axis=0))
        top_lift = rs.nlargest(10, 'lift')
        lift['top10'].append(top_lift['lift'].mean(axis=0))

    _, axs = plt.subplots(1, 2, figsize=(10, 5), squeeze=False)
    ds.multiple_line_chart(metric_values, conf, ax=axs[0, 0], title=f'Avg Confidence x {metric}',
                           xlabel=metric, ylabel='Avg confidence')
    ds.multiple_line_chart(metric_values, lift, ax=axs[0, 1], title=f'Avg Lift x {metric}',
                           xlabel=metric, ylabel='Avg lift')
    plt.show()

    plot_top_rules(top_conf, 'confidence', metric)
    plot_top_rules(top_lift, 'lift', metric)

    return nr_rules

#### Setup ###
pd.plotting.register_matplotlib_converters()

path = os.getcwd() 
path = os.path.abspath(os.path.join(path, os.pardir))
hfp_path = path + "\datasets\heart_failure_clinical_records_dataset.csv"
qsar_path = path + "\datasets\qsar_oral_toxicity.csv"

print("Path:", path)
print("Path HFP:", hfp_path)

hfp_data = pd.read_csv(hfp_path, sep=',', decimal='.', parse_dates=True, infer_datetime_format=True)
numeric_vars_hfp = hfp_data.select_dtypes(include='number').columns
#print(hfp_data)

qsar_data = pd.read_csv(qsar_path, sep=';')
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns
#print(qsar_data)

############################################
###############Data Profiling###############
############################################

####################
#Data Dimensionality
####################

print('Hearth failure clinical records dataset: ','nr records = ', hfp_data.shape[0], 'nr variables (Dimensionality) = ', hfp_data.shape[1])
plt.figure(figsize=(4,2))
values = {'nr records': hfp_data.shape[0], 'nr variables': hfp_data.shape[1]}
ds.bar_chart(values.keys(), values.values(), title = 'Nr of records vs nr of variables in the hfp dataset')
plt.show()

print('Oral toxicity dataset: ', 'nr records = ', 'nr records =', qsar_data.shape[0], 'nr variables (Dimensionality) = ', qsar_data.shape[1])
plt.figure(figsize=(4,2))
values = {'nr records': qsar_data.shape[0], 'nr variables': qsar_data.shape[1]}
ds.bar_chart(values.keys(), values.values(), title='Nr of records vs nr of variables in the qsar dataset')
plt.show()

##################
#Data Distribution
##################

print("\n\n\n     printing Data Distribution\n")

print("hfp_data type: " , hfp_data.dtypes)
cat_vars_hfp = hfp_data.select_dtypes(include='object')
hfp_data[cat_vars_hfp.columns] = hfp_data.select_dtypes(['object']).apply(lambda x: x.astype('category'))
#print("hfp_data type: " , hfp_data.dtypes)

print("qsar_data type: " , qsar_data.dtypes)
cat_vars_qsar = qsar_data.select_dtypes(include='object')
qsar_data[cat_vars_qsar.columns] = qsar_data.select_dtypes(['object']).apply(lambda x: x.astype('category'))
#print("qsar_data type: " , qsar_data.dtypes)

hfp_data.describe()
qsar_data.describe()

######### Missing Values

#Missing values of the hearth failure clinical records dataset
"""
plt.figure()
missingValues = {}
for var in hfp_data:
    missingValues[var] = hfp_data[var].isna().sum()
ds.bar_chart(missingValues.keys(), missingValues.values(), title='Nr of missing values per variable in hfp_data', xlabel='variables', ylabel='nr missing values')
print('hfp missing values ==> ', missingValues.values())
plt.show()
"""
#Missing values of the oral toxicity dataset
"""
plt.figure()
missingValues = {}
for var in qsar_data:
    missingValues[var] = qsar_data[var].isna().sum()
ds.bar_chart(missingValues.keys(), missingValues.values(), title='Nr of missing values per variable in qsar_data', xlabel='variables', ylabel='nr missing values')
print('qsar missing values ==> ', missingValues.values())
plt.show()
"""

######### Boxplot

#Boxplots of the hearth failure clinical records dataset
hfp_data.boxplot(rot=45)
plt.show()

nonBinNum = 0
for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) > 2):
        nonBinNum = nonBinNum + 1

rows_NonBin, cols_NonBin = ds.choose_grid(nonBinNum)
fig, axs = plt.subplots(rows_NonBin, cols_NonBin, figsize = (cols_NonBin*ds.HEIGHT, rows_NonBin*ds.HEIGHT))
i, j = 0, 0
nonBin_n = 0
for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) > 2):
        axs[i, j].set_title('Boxplot for %s'%numeric_vars_hfp[n])
        axs[i, j].boxplot(hfp_data[numeric_vars_hfp[n]].dropna().values)
        i, j = (i + 1, 0) if (nonBin_n + 1) % cols_NonBin == 0 else (i, j + 1)
        nonBin_n = nonBin_n + 1
plt.show()

#Boxplots of the oral toxicity dataset
"""
qsar_data.boxplot(rot=45)
plt.show()

rows, cols = ds.choose_grid(len(numeric_vars_qsar))
fig, axs = plt.subplots(rows, cols, figsize=(cols*ds.HEIGHT, rows*ds.HEIGHT))
i, j = 0, 0
for n in range(len(numeric_vars_qsar)):
    axs[i, j].set_title('Boxplot for %s'%numeric_vars_qsar[n])
    axs[i, j].boxplot(qsar_data[numeric_vars_qsar[n]].dropna().values)
    i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
plt.show()
"""

######### Histograms

#Histograms for the hearth failure clinical records dataset
nonBinNum = 0
i_NonBin, j_NonBin = 0, 0
i_Bin, j_Bin = 0, 0
n_NonBin, n_Bin = 0, 0

for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) > 2):
        nonBinNum = nonBinNum + 1

rows_NonBin, cols_NonBin = ds.choose_grid(nonBinNum)
rows_Bin, cols_Bin = ds.choose_grid(len(numeric_vars_hfp) - nonBinNum)

plt.figure()
fig_NonBin, axs_NonBin = plt.subplots(rows_NonBin, cols_NonBin, figsize=(cols_NonBin*ds.HEIGHT, rows_NonBin*ds.HEIGHT))



for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) > 2):
        axs_NonBin[i_NonBin, j_NonBin].set_title('Histogram for %s'%numeric_vars_hfp[n])
        axs_NonBin[i_NonBin, j_NonBin].set_xlabel(numeric_vars_hfp[n])
        axs_NonBin[i_NonBin, j_NonBin].set_ylabel('nr records')
        axs_NonBin[i_NonBin, j_NonBin].hist(hfp_data[numeric_vars_hfp[n]].values, bins=different(hfp_data[numeric_vars_hfp[n]].values))
        i_NonBin, j_NonBin = (i_NonBin + 1, 0) if (n_NonBin+1) % cols_NonBin == 0 else (i_NonBin, j_NonBin + 1)
        n_NonBin = n_NonBin + 1

plt.show()


plt.figure()
fig_Bin, axs_Bin = plt.subplots(rows_Bin, cols_Bin, figsize=(cols_Bin*ds.HEIGHT, rows_Bin*ds.HEIGHT))

for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) <= 2):
        hfp_counts = hfp_data[hfp_data.columns[n]]
        hfp_counts = hfp_counts.value_counts()

        axs_Bin[i_Bin, j_Bin].set_title('Histogram for %s' % numeric_vars_hfp[n])
        axs_Bin[i_Bin, j_Bin].set_xlabel(numeric_vars_hfp[n])
        axs_Bin[i_Bin, j_Bin].set_ylabel('nr records')
        axs_Bin[i_Bin, j_Bin].bar(["False", "True"], hfp_counts.values)
        i_Bin, j_Bin = (i_Bin + 1, 0) if (n_Bin + 1) % cols_Bin == 0 else (i_Bin, j_Bin + 1)
        n_Bin = n_Bin + 1


        """hfp_counts = hfp_data[hfp_data.columns[n]]
        hfp_counts = hfp_counts.value_counts()
        ds.bar_chart(hfp_counts.index, hfp_counts.values, title = 'Histogram for %s'%numeric_vars_hfp[n])
        print('hfp_counts.index = ', hfp_counts.index, '\n hfp_counts.values = ', hfp_counts.values, '\n')"""

plt.show()

#Histograms for the oral toxicity dataset

plt.figure()
counts = qsar_data[qsar_data.columns[-1]]
counts = counts.value_counts()
ds.bar_chart(counts.index, counts.values, title='qsar distribution')
plt.show()


plt.figure()
positive_qsar = qsar_data[qsar_data[qsar_data.columns[-1]] == 'positive']
negative_qsar = qsar_data[qsar_data[qsar_data.columns[-1]] == 'negative']

sum_pq=positive_qsar.sum(axis=1)
ds.bar_chart(["positive", "negative"], [positive_qsar.sum(axis=1).mean(axis=0), negative_qsar.sum(axis=1).mean(axis=0)], title='qsar medium 1s')
plt.show()

#Histograms for the oral toxicity dataset
"""
rows, cols = ds.choose_grid(len(numeric_vars_qsar))
fig, axs = plt.subplots(rows, cols, figsize=(cols*ds.HEIGHT, rows*ds.HEIGHT))
i, j = 0, 0
for n in range(len(numeric_vars_qsar)):
    axs[i, j].set_title('Histogram for %s'%numeric_vars_qsar[n])
    axs[i, j].set_xlabel(numeric_vars_qsar[n])
    axs[i, j].set_ylabel('nr records')
    axs[i, j].hist(qsar_data[numeric_vars_qsar[n]].values, bins=different(qsar_data[numeric_vars_qsar[n]].values))
    i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
plt.show()
"""
######### Discrete Distribution

# Histogram with Trend for the heart failure clinical records dataset

nonBinNum = 0
for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) > 2):
        nonBinNum = nonBinNum + 1

rows_NonBin, cols_NonBin = ds.choose_grid(nonBinNum)
fig, axs = plt.subplots(rows_NonBin, cols_NonBin, figsize=(cols_NonBin*ds.HEIGHT, rows_NonBin*ds.HEIGHT))
i, j = 0, 0
n_NonBin = 0

for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) > 2):
        axs[i, j].set_title('Histogram with trend for %s'%numeric_vars_hfp[n])
        sns.distplot(hfp_data[numeric_vars_hfp[n]].dropna().values, norm_hist=True, ax=axs[i, j], axlabel=numeric_vars_hfp[n])
        i, j = (i + 1, 0) if (n_NonBin + 1) % cols_NonBin == 0 else (i, j + 1)
        n_NonBin = n_NonBin + 1
plt.show()

# Histogram with Trend for the oral toxicity dataset
"""
rows, cols = ds.choose_grid(len(numeric_vars_qsar))
fig, axs = plt.subplots(rows, cols, figsize=(cols*ds.HEIGHT, rows*ds.HEIGHT))
i, j = 0, 0
for n in range(len(numeric_vars_qsar)):
    axs[i, j].set_title('Histogram with trend for %s'%numeric_vars_qsar[n])
    sns.distplot(qsar_data[numeric_vars_hfp[n]].dropna().values, norm_hist=True, ax=axs[i, j], axlabel=numeric_vars_qsar[n])
    i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
plt.show()
"""
######### Histogram with various distributions

def compute_known_distributions(x_values: list) -> dict:
    distributions = dict()
    # Gaussian
    mean, sigma = _stats.norm.fit(x_values)
    distributions['Normal(%.1f,%.2f)'%(mean,sigma)] = _stats.norm.pdf(x_values, mean, sigma)
    # Exponential
    loc, scale = _stats.expon.fit(x_values)
    distributions['Exp(%.2f)'%(1/scale)] = _stats.expon.pdf(x_values, loc, scale)
    # LogNorm
    sigma, loc, scale = _stats.lognorm.fit(x_values)
    distributions['LogNor(%.1f,%.2f)'%(np.log(scale),sigma)] = _stats.lognorm.pdf(x_values, sigma, loc, scale)
    return distributions

def histogram_with_distributions(ax: plt.Axes, series: pd.Series, var: str):
    values = series.sort_values().values
    ax.hist(values, 20, density=True)
    distributions = compute_known_distributions(values)
    ds.multiple_line_chart(values, distributions, ax=ax, title='Best fit for %s'%var, xlabel=var, ylabel='')

#Histogram with various distributions for the hearth failure clinical records dataset
fig, axs = plt.subplots(rows_NonBin, cols_NonBin, figsize=(cols_NonBin*ds.HEIGHT, rows_NonBin*ds.HEIGHT))
i, j = 0, 0
n_NonBin = 0
for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) > 2):
        histogram_with_distributions(axs[i, j], hfp_data[numeric_vars_hfp[n]].dropna(), numeric_vars_hfp[n])
        i, j = (i + 1, 0) if (n_NonBin + 1) % cols_NonBin == 0 else (i, j + 1)
        n_NonBin = n_NonBin + 1
plt.show()

##################
#Data Granularity
##################

print('Data Granularity already done in the Histograms from the Data Distribution Part')

##############
#Data Sparsity
##############

print("\n\n\n     printing Data Sparsity\n")

nonBinNum = 0

for n in range(len(numeric_vars_hfp)):
    if(different(hfp_data[numeric_vars_hfp[n]].values) > 2):
        nonBinNum = nonBinNum + 1

rows_NonBin, cols_NonBin = nonBinNum-1, nonBinNum-1

register_matplotlib_converters()

hfp_columns = hfp_data.select_dtypes(include='number').columns

i_NonBin, j_NonBin = 0, 0

plt.figure()
fig, axs = plt.subplots(rows_NonBin, cols_NonBin, figsize=(cols_NonBin*4, rows_NonBin*4), squeeze=False)
for i in range(len(hfp_columns)):
    j_NonBin = 0
    if(different(hfp_data[numeric_vars_hfp[i]].values) > 2):
        var1 = hfp_columns[i]
        for j in range(i+1, len(hfp_columns)):
            if(different(hfp_data[numeric_vars_hfp[j]].values) > 2):
                var2 = hfp_columns[j]
                axs[i_NonBin, j_NonBin].set_title("%s x %s"%(var1,var2))
                axs[i_NonBin, j_NonBin].set_xlabel(var1)
                axs[i_NonBin, j_NonBin].set_ylabel(var2)
                axs[i_NonBin, j_NonBin].scatter(hfp_data[var1], hfp_data[var2])
                j_NonBin = j_NonBin + 1
        i_NonBin = i_NonBin + 1

plt.show()

#####################
#Correlation Analysis
#####################

print("\n\n\n     printing Correlation Analysis\n")

fig = plt.figure(figsize=[12, 12])
corr_mtx = hfp_data.corr()
sns.heatmap(corr_mtx, xticklabels=corr_mtx.columns, yticklabels=corr_mtx.columns, annot=True, cmap='Reds')
plt.title('Correlation analysis for the hearth failure clinical records dataset')
plt.show()

"""
fig = plt.figure(figsize=[12, 12])
corr_mtx = qsar_data.corr()
sns.heatmap(corr_mtx, xticklabels=corr_mtx.columns, yticklabels=corr_mtx.columns, annot=True, cmap='Reds')
plt.title('Correlation analysis for the oral toxicity dataset')
plt.show()
"""


####################################################
##################Data Preparation##################
####################################################

###########
#Imputation
###########

print("\n\n\n     doing Data Preparation\n")

#Hearth Failure Clinical Records dataset
hfp_data.describe(include='all')
hfp_imp = SimpleImputer(strategy='mean', fill_value=-1, missing_values=np.nan, copy=True)
hfp_imp.fit(hfp_data.values)
hfp_mat = hfp_imp.transform(hfp_data.values)
hfp_df = pd.DataFrame(hfp_mat, columns = hfp_data.columns)
hfp_df.describe(include='all')

#Oral Toxicity dataset
qsar_sb_vars = qsar_data.select_dtypes(include='object')
qsar_data[qsar_sb_vars.columns] = qsar_data.select_dtypes(['object']).apply(lambda x: x.astype('category'))
qsar_cols_nr = qsar_data.select_dtypes(include='number')
qsar_cols_sb = qsar_data.select_dtypes(include='category')
qsar_data.describe(include='all')

qsar_imp = SimpleImputer(strategy='constant', fill_value='NA', missing_values=np.nan, copy=True)
qsar_imp.fit(qsar_data.values)
qsar_mat = qsar_imp.transform(qsar_data.values)
qsar_dataFrame = pd.DataFrame(qsar_mat, columns=qsar_data.columns)
qsar_dataFrame.describe(include='all')

qsar_imp_nr = SimpleImputer(strategy='mean', missing_values=np.nan, copy=True)
qsar_imp_sb = SimpleImputer(strategy='most_frequent', missing_values='', copy=True)
qsar_df_nr = pd.DataFrame(qsar_imp_nr.fit_transform(qsar_cols_nr), columns=qsar_cols_nr.columns)
qsar_df_sb = pd.DataFrame(qsar_imp_sb.fit_transform(qsar_cols_sb), columns=qsar_cols_sb.columns)

qsar_dataFrame = qsar_df_nr.join(qsar_df_sb, how='right')
qsar_dataFrame.describe(include='all')

# Outliers Imputation for the Heart Failure Clinical Records dataset

actual_df_hfp = hfp_data

for n in range(len(hfp_columns)):
    if(different(actual_df_hfp[numeric_vars_hfp[n]].values) > 2):
        Q1 = actual_df_hfp[numeric_vars_hfp[n]].quantile(0.25)
        Q3 = actual_df_hfp[numeric_vars_hfp[n]].quantile(0.75)
        IQR = Q3 - Q1
        print('IQR = ', IQR)
        df_remove_hfp = actual_df_hfp.loc[(hfp_data[numeric_vars_hfp[n]] < Q1-1.5*IQR) | (hfp_data[numeric_vars_hfp[n]] > Q3+1.5*IQR)]
        actual_df_hfp = actual_df_hfp.drop(df_remove_hfp.index)

hfp_df = actual_df_hfp
print(hfp_df)

##########
#Balancing
##########

print("\n\n\n     printing Balancing\n")

#Balancing for the Hearth Failure Clinical Records dataset
"""
hfp_data = hfp_df
numeric_vars_hfp = hfp_data.select_dtypes(include='number').columns
hfp_target_count = hfp_data['DEATH_EVENT'].value_counts()
plt.figure()
plt.title('Class balance for the Hearth Failure Clinical Records dataset')
plt.bar(hfp_target_count.index, hfp_target_count.values)
plt.show()

hfp_min_class = hfp_target_count.idxmin()
hfp_ind_min_class = hfp_target_count.index.get_loc(hfp_min_class)
plt.show()
print('Minority class:', hfp_target_count[hfp_ind_min_class])
print('Majority class:', hfp_target_count[1 - hfp_ind_min_class])
print('Proportion:', round(hfp_target_count[hfp_ind_min_class] / hfp_target_count[1 - hfp_ind_min_class], 2), ': 1')

RANDOM_STATE = 42
hfp_values = {'Original': [hfp_target_count.values[hfp_ind_min_class], hfp_target_count.values[1 - hfp_ind_min_class]]}

hfp_df_class_min = hfp_data[hfp_data['DEATH_EVENT'] == hfp_min_class]
hfp_df_class_max = hfp_data[hfp_data['DEATH_EVENT'] != hfp_min_class]

hfp_df_under = hfp_df_class_max.sample(len(hfp_df_class_min))
hfp_values['UnderSample'] = [hfp_target_count.values[hfp_ind_min_class], len(hfp_df_under)]

hfp_df_over = hfp_df_class_min.sample(len(hfp_df_class_max), replace=True)
hfp_values['OverSample'] = [len(hfp_df_over), hfp_target_count.values[1 - hfp_ind_min_class]]

hfp_smote = SMOTE(sampling_strategy = 'minority', random_state = RANDOM_STATE)
hfp_new = hfp_data
hfp_y = hfp_new.pop('DEATH_EVENT').values
hfp_X = hfp_data.values
hfp_smote_X, hfp_smote_y = hfp_smote.fit_sample(hfp_X, hfp_y)
hfp_smote_target_count = pd.Series(hfp_smote_y).value_counts()
hfp_values['SMOTE'] = [hfp_smote_target_count.values[hfp_ind_min_class], hfp_smote_target_count.values[1 - hfp_ind_min_class]]

fig = plt.figure()
ds.multiple_bar_chart([hfp_target_count.index[hfp_ind_min_class], hfp_target_count.index[1 - hfp_ind_min_class]], hfp_values,
                      title='Target for the Heart Failure Clinical Records dataset', xlabel='frequency', ylabel='Class balance')
plt.show()

#Balancing for the Oral Toxicity dataset

qsar_data = qsar_dataFrame
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns
qsar_target_count = qsar_data['negative'].value_counts()
plt.figure()
plt.title('Class balance for the Oral Toxicity dataset')
plt.bar(qsar_target_count.index, qsar_target_count.values)
plt.show()

qsar_min_class = qsar_target_count.idxmin()
qsar_ind_min_class = qsar_target_count.index.get_loc(qsar_min_class)
plt.show()
print('Minority class:', qsar_target_count[qsar_ind_min_class])
print('Majority class:', qsar_target_count[1 - qsar_ind_min_class])
print('Proportion:', round(qsar_target_count[qsar_ind_min_class] / qsar_target_count[1 - qsar_ind_min_class], 2), ': 1')

RANDOM_STATE = 42
qsar_values = {'Original': [qsar_target_count.values[qsar_ind_min_class], qsar_target_count.values[1 - qsar_ind_min_class]]}

qsar_df_class_min = qsar_data[qsar_data['negative'] == qsar_min_class]
qsar_df_class_max = qsar_data[qsar_data['negative'] != qsar_min_class]

qsar_df_under = qsar_df_class_max.sample(len(qsar_df_class_min))
qsar_values['UnderSample'] = [qsar_target_count.values[qsar_ind_min_class], len(qsar_df_under)]

qsar_df_over = qsar_df_class_min.sample(len(qsar_df_class_max), replace=True)
qsar_values['OverSample'] = [len(qsar_df_over), qsar_target_count.values[1 - qsar_ind_min_class]]

qsar_smote = SMOTE(sampling_strategy = 'minority', random_state = RANDOM_STATE)
qsar_y = qsar_data.pop('negative').values
qsar_X = qsar_data.values
qsar_smote_X, qsar_smote_y = qsar_smote.fit_sample(qsar_X, qsar_y)
qsar_smote_target_count = pd.Series(qsar_smote_y).value_counts()
qsar_values['SMOTE'] = [qsar_smote_target_count.values[qsar_ind_min_class], qsar_smote_target_count.values[1 - qsar_ind_min_class]]

fig = plt.figure()
ds.multiple_bar_chart([qsar_target_count.index[qsar_ind_min_class], qsar_target_count.index[1 - qsar_ind_min_class]], qsar_values,
                      title='Target for the Oral Toxicity dataset', xlabel='frequency', ylabel='Class balance')
plt.show()

hfp_df = hfp_df_over
"""
#####################
#Correlation Analysis
#####################

print("\n\n\n     printing Correlation Analysis\n")

fig = plt.figure(figsize=[12, 12])
corr_mtx = hfp_df.corr()
sns.heatmap(corr_mtx, xticklabels=corr_mtx.columns, yticklabels=corr_mtx.columns, annot=True, cmap='Reds')
plt.title('Correlation analysis for the hearth failure clinical records dataset')
plt.show()

########
#Scaling
########

print("\n\n\n     printing Scaling\n")

#Scaling for Heart Failure Clinical Records dataset

#########StandardScaler#########
hfp_transf = StandardScaler(with_mean=True, with_std=True, copy=True).fit(hfp_df)
hfp_df = pd.DataFrame(hfp_transf.transform(hfp_df), columns = hfp_df.columns)
hfp_norm_data_zscore = hfp_df
hfp_norm_data_zscore.describe(include='all')

##########MinMaxScaler##########
hfp_transf = MinMaxScaler(feature_range=(0, 1), copy=True).fit(hfp_df)
hfp_df = pd.DataFrame(hfp_transf.transform(hfp_df), columns= hfp_df.columns)
hfp_norm_data_minmax = hfp_df
hfp_norm_data_minmax.describe(include='all')

hfp_fig, hfp_axs = plt.subplots(1, 3, figsize=(20,10),squeeze=False)
hfp_axs[0, 0].set_title('Original data')
hfp_df.boxplot(ax=hfp_axs[0, 0])
hfp_axs[0, 1].set_title('Z-score normalization')
hfp_norm_data_zscore.boxplot(ax = hfp_axs[0, 1])
hfp_axs[0, 2].set_title('MinMax normalization')
hfp_norm_data_minmax.boxplot(ax = hfp_axs[0, 2])
hfp_fig.tight_layout()
plt.show()

general_hfp_df = hfp_df
print(hfp_df)

#Scaling for Oral Toxicity dataset

#########StandardScaler#########
"""
qsar_transf = StandardScaler(with_mean=True, with_std=True, copy=True).fit(qsar_df_nr)
qsar_dataFrame = pd.DataFrame(qsar_transf.transform(qsar_df_nr), columns = qsar_df_nr.columns)
qsar_norm_data_zscore = qsar_dataFrame
qsar_norm_data_zscore.describe(include='all')
"""
##########MinMaxScaler##########
"""
qsar_transf = MinMaxScaler(feature_range=(0, 1), copy=True).fit(qsar_df_nr)
qsar_dataFrame = pd.DataFrame(qsar_transf.transform(qsar_df_nr), columns = qsar_df_nr.columns)
qsar_norm_data_minmax = qsar_dataFrame
qsar_norm_data_minmax.describe(include='all')

qsar_fig, qsar_axs = plt.subplots(1, 3, figsize=(20,10),squeeze=False)
qsar_axs[0, 0].set_title('Original data')
qsar_dataFrame.boxplot(ax=qsar_axs[0, 0])
qsar_axs[0, 1].set_title('Z-score normalization')
qsar_norm_data_zscore.boxplot(ax = qsar_axs[0, 1])
qsar_axs[0, 2].set_title('MinMax normalization')
qsar_norm_data_minmax.boxplot(ax = qsar_axs[0, 2])
qsar_fig.tight_layout()
plt.show()
"""

####################
#Feature Engineering
####################

#Estou a ver os videos sobre isso.

#Para o Feature Selection do Heart Failure Clinical Records dataset, não precisamos de retirar nenhuma variável, visto verificarmos 
# através do grafico de Correlation Analisis que não à variáveis que se sobrepõem

#Feature Selection for the Oral Toxicity dataset
"""
#qsar_data = qsar_df_under
qsar_data = qsar_dataFrame
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns
print(qsar_data)

new_qsar_data = qsar_data
new_numeric_vars_qsar = numeric_vars_qsar

qsar_dim = len(numeric_vars_qsar)
qsar_k = 100

corr_mtx = qsar_data.corr()
new_corr_mtx = corr_mtx
print('corr_mtx = ', new_corr_mtx)
i, j = 0, 0
worst = 1.0

#Sequential Backward Seletion (SBS)
while worst > 0.8:
    #new_corr_mtx = new_qsar_data.corr()
    worst = 0.8
    i_selected, j_selected = 0, 0
    for i in range(len(new_corr_mtx.columns)):
        actual_pos = 0
        for j in new_corr_mtx[new_corr_mtx.columns[i]]:
            if j > worst and actual_pos != i:
                worst = j
                i_selected = i
                j_selected = j
            actual_pos = actual_pos + 1
    new_qsar_data.pop(new_qsar_data.columns[i_selected])
    for r in range(len(new_corr_mtx[new_corr_mtx.columns[i_selected]])):
        #print('aqui\n')
        new_corr_mtx[new_corr_mtx.columns[i_selected]][r] = 0.0
    for c in range(len(new_corr_mtx.columns)):
        #print('aqui\n')
        new_corr_mtx[new_corr_mtx.columns[c]][i_selected] = 0.0
    print(len(new_qsar_data.columns))
    print(worst, ' ', i_selected)

new_numeric_vars_qsar = new_qsar_data.select_dtypes(include='number').columns

numeric_vars_qsar = new_numeric_vars_qsar
qsar_data = new_qsar_data
#qsar_df_under = new_qsar_data
qsar_dataFrame = new_qsar_data
"""
# Feature Generation for the Heart Failure Clinical Records dataset

#hfp_data = hfp_df_over
hfp_data = hfp_df
hfp_length = len(hfp_data['age'])

plat_array = []

for n in hfp_data.index:
    percentage_blood = hfp_data['ejection_fraction'][n]
    plat = hfp_data['platelets'][n]
    plat_array.append(plat * percentage_blood) 


hfp_data['platelets_ejection_fraction'] = pd.Series(plat_array, index=hfp_data.index)

print(hfp_data)
#hfp_df_over = hfp_data
hfp_df = hfp_data

############
#Naive Bayes
############

print("\n\n\n     printing Naive Bayes\n")

#Naive Bayes for Heart Failure Clinical Records dataset

hfp_data = hfp_df

hfp_y: np.ndarray = hfp_data.pop('DEATH_EVENT').values
hfp_X: np.ndarray = hfp_data.values

hfp_labels = pd.unique(hfp_y)
hfp_trnX, hfp_tstX, hfp_trnY, hfp_tstY = train_test_split(hfp_X, hfp_y, train_size=0.7, stratify = hfp_y)
hfp_clf = GaussianNB()
hfp_clf.fit(hfp_trnX, hfp_trnY)
hfp_prd_trn = hfp_clf.predict(hfp_trnX)
hfp_prd_tst = hfp_clf.predict(hfp_tstX)
ds.plot_evaluation_results(pd.unique(hfp_y), hfp_trnY, hfp_prd_trn, hfp_tstY, hfp_prd_tst)

hfp_estimators = {'GaussianNB': GaussianNB(), 'MultinomialNB': MultinomialNB(), 'BernoulyNB': BernoulliNB()}
hfp_xvalues = []
hfp_yvalues = []

var = LeaveOneOut()

for clf in hfp_estimators:
    hfp_xvalues.append(clf)
    cv_scores = cross_val_score(clf, hfp_X, hfp_y, cv = var, scoring='accuracy')
    hfp_yvalues.append(np.mean(cv_scores))

plt.figure()
ds.bar_chart(hfp_xvalues, hfp_yvalues, title='Comparison of Naive Bayes Models', ylabel='accuracy', percentage=True)
plt.show()

#Naive Bayes for Oral Toxicity dataset

qsar_data = qsar_df_under

qsar_y: np.ndarray = qsar_data.pop('negative').values
qsar_X: np.ndarray = qsar_data.values

qsar_labels = pd.unique(qsar_y)
qsar_trnX, qsar_tstX, qsar_trnY, qsar_tstY = train_test_split(qsar_X, qsar_y, train_size=0.7, stratify = qsar_y)
qsar_clf = GaussianNB()
qsar_clf.fit(qsar_trnX, qsar_trnY)
qsar_prd_trn = qsar_clf.predict(qsar_trnX)
qsar_prd_tst = qsar_clf.predict(qsar_tstX)
ds.plot_evaluation_results(pd.unique(qsar_y), qsar_trnY, qsar_prd_trn, qsar_tstY, qsar_prd_tst)

qsar_estimators = {'GaussianNB': GaussianNB(), 'MultinomialNB': MultinomialNB(), 'BernoulyNB': BernoulliNB()}
qsar_xvalues = []
qsar_yvalues = []
var = LeaveOneOut()

for clf in qsar_estimators:
    qsar_xvalues.append(clf)
    qsar_estimators[clf].fit(qsar_trnX, qsar_trnY)
    qsar_prdY = qsar_estimators[clf].predict(qsar_tstX)
    qsar_yvalues.append(metrics.accuracy_score(qsar_tstY, qsar_prdY))

plt.figure()
ds.bar_chart(qsar_xvalues, qsar_yvalues, title='Comparison of Naive Bayes Models', ylabel='accuracy', percentage=True)
plt.show()

##################
#Gradient Boosting
##################

#Gradient Boosting for the Heart Failure Clinical Records dataset

hfp_data = hfp_df
numeric_vars_hfp = hfp_data.select_dtypes(include='number').columns
hfp_y: np.ndarray = hfp_data.pop('DEATH_EVENT').values
hfp_X: np.ndarray = hfp_data.values
hfp_labels = pd.unique(hfp_y)

hfp_trnX, hfp_tstX, hfp_trnY, hfp_tstY = train_test_split(hfp_X, hfp_y, train_size = 0.7, stratify = hfp_y)

hfp_n_estimators = [5, 10, 25, 50, 75, 100, 150, 200, 250, 300]
hfp_max_depths = [5, 10, 25]
hfp_learning_rate = [.1, .3, .5, .7, .9]
hfp_best = ('', 0, 0)
hfp_last_best = 0
hfp_best_tree = None

hfp_cols = len(hfp_max_depths)
plt.figure()
hfp_fig, hfp_axs = plt.subplots(1, hfp_cols, figsize = (hfp_cols*ds.HEIGHT, ds.HEIGHT), squeeze = False)

var = LeaveOneOut()

for k in range(len(hfp_max_depths)):
    d = hfp_max_depths[k]
    hfp_values = {}
    for lr in hfp_learning_rate:
        hfp_yvalues = []
        for n in hfp_n_estimators:
            hfp_gb = GradientBoostingClassifier(n_estimators = n, max_depth = d, learning_rate = lr)
            cv_scores = cross_val_score(hfp_gb, hfp_X, hfp_y, cv = var, scoring='accuracy')
            hfp_yvalues.append(np.mean(cv_scores))
            if hfp_yvalues[-1] > hfp_last_best:
                hfp_best = (d, lr, n)
                hfp_last_best = hfp_yvalues[-1]
                hfp_best_tree = hfp_gb
        hfp_values[lr] = hfp_yvalues
    ds.multiple_line_chart(hfp_n_estimators, hfp_values, ax = hfp_axs[0, k], 
                           title='Gradient Boorsting with max_depth=%d'%d,
                           xlabel='nr estimators', ylabel='accuracy', percentage=True)

plt.show()
print('Best results for the Heart Failure Clinical Records dataset with depth=%d, learning rate=%1.2f and %d estimators, with accuracy=%1.2f'%(
    hfp_best[0], hfp_best[1], hfp_best[2], hfp_last_best))

#Gradient Boosting for the Oral Toxicity dataset

qsar_data = qsar_df_under
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns

qsar_df_nr = pd.DataFrame(qsar_imp_nr.fit_transform(qsar_cols_nr), columns=qsar_cols_nr.columns)
qsar_df_sb = pd.DataFrame(qsar_imp_sb.fit_transform(qsar_cols_sb), columns=qsar_cols_sb.columns)
qsar_dataFrame = qsar_df_nr.join(qsar_df_sb, how='right')
qsar_dataFrame.describe(include='all')

qsar_y: np.ndarray = qsar_df_nr.pop('0.970').values
qsar_X: np.ndarray = qsar_df_nr.values
qsar_labels = pd.unique(qsar_y)

qsar_trnX, qsar_tstX, qsar_trnY, qsar_tstY = train_test_split(qsar_X, qsar_y, train_size = 0.7, stratify = qsar_y)

qsar_n_estimators = [5, 10, 25, 50, 75, 100, 150, 200, 250, 300]
qsar_max_depths = [5, 10, 25]
qsar_learning_rate = [.1, .3, .5, .7, .9]
qsar_best = ('', 0, 0)
qsar_last_best = 0
qsar_best_tree = None

qsar_cols = len(qsar_max_depths)
plt.figure()
qsar_fig, qsar_axs = plt.subplots(1, qsar_cols, figsize = (qsar_cols*ds.HEIGHT, ds.HEIGHT), squeeze = False)
for k in range(len(qsar_max_depths)):
    d = qsar_max_depths[k]
    qsar_values = {}
    for lr in qsar_learning_rate:
        qsar_yvalues = []
        for n in qsar_n_estimators:
            qsar_gb = GradientBoostingClassifier(n_estimators = n, max_depth = d, learning_rate = lr)
            qsar_gb.fit(qsar_trnX, qsar_trnY)
            qsar_prdY = qsar_gb.predict(qsar_tstX)
            qsar_yvalues.append(metrics.accuracy_score(qsar_tstY, qsar_prdY))
            if qsar_yvalues[-1] > qsar_last_best:
                qsar_best = (d, lr, n)
                qsar_last_best = qsar_yvalues[-1]
                qsar_best_tree = qsar_gb
        qsar_values[lr] = qsar_yvalues
    ds.multiple_line_chart(qsar_n_estimators, qsar_values, ax = qsar_axs[0, k], 
                           title='Gradient Boorsting with max_depth=%d'%d,
                           xlabel='nr estimators', ylabel='accuracy', percentage=True)

plt.show()
print('Best results for the Heart Failure Clinical Records dataset with depth=%d, learning rate=%1.2f and %d estimators, with accuracy=%1.2f'%(
    qsar_best[0], qsar_best[1], qsar_best[2], qsar_last_best))

####
#KNN
####

print("\n\n\n     printing KNN\n")

#KNN for Heart Failure Clinical Records dataset

hfp_data = hfp_df
hfp_y = hfp_data.pop('DEATH_EVENT').values
hfp_X = hfp_data.values
#numeric_vars_hfp = hfp_data.select_dtypes(include='number').columns
labels = pd.unique(hfp_y)

hfp_trnX, hfp_tstX, hfp_trnY, hfp_tstY = train_test_split(hfp_X, hfp_y, train_size = 0.7, stratify = hfp_y)

hfp_nvalues = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
hfp_dist = ['manhattan', 'euclidean', 'chebyshev']
hfp_values = {}
hfp_best = (0, '')
hfp_last_best = 0

count = 0
#loo = LeaveOneOut()

for d in hfp_dist:
    hfp_yvalues = []
    for n in hfp_nvalues:
        hfp_knn = KNeighborsClassifier(n_neighbors = n, metric = d)
        cv_scores = cross_val_score(hfp_knn, hfp_X, hfp_y, cv= 112, scoring='accuracy')
        hfp_yvalues.append(np.mean(cv_scores))
        if hfp_yvalues[-1] > hfp_last_best:
            hfp_best = (n, d)
            hfp_last_best = hfp_yvalues[-1]
    hfp_values[d] = hfp_yvalues

plt.figure()
ds.multiple_line_chart(hfp_nvalues, hfp_values, title='KNN variants', xlabel='n', ylabel='accuracy', percentage=True)
plt.show()
print('Best results with %d neighbors and %s'%(hfp_best[0], hfp_best[1]))

hfp_clf = hfp_knn = KNeighborsClassifier(n_neighbors = hfp_best[0], metric = hfp_best[1])
hfp_clf.fit(hfp_trnX, hfp_trnY)
hfp_prd_trn = hfp_clf.predict(hfp_trnX)
hfp_prd_tst = hfp_clf.predict(hfp_tstX)
ds.plot_evaluation_results(pd.unique(hfp_y), hfp_trnY, hfp_prd_trn, hfp_tstY, hfp_prd_tst)
plt.show()

#KNN for Oral Toxicity dataset

qsar_data = qsar_dataFrame
qsar_y = qsar_data.pop('negative').values
qsar_X = qsar_data.values
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns
labels = pd.unique(qsar_y)

qsar_trnX, qsar_tstX, qsar_trnY, qsar_tstY = train_test_split(qsar_X, qsar_y, train_size = 0.7, stratify = qsar_y)

qsar_nvalues = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
qsar_dist = ['manhattan', 'euclidean', 'chebyshev']
qsar_values = {}
qsar_best = (0, '')
qsar_last_best = 0
for d in qsar_dist:
    qsar_yvalues = []
    for n in qsar_nvalues:
        qsar_knn = KNeighborsClassifier(n_neighbors = n, metric = d)
        qsar_knn.fit(qsar_trnX, qsar_trnY)
        qsar_prdY = qsar_knn.predict(qsar_tstX)
        cv_scores = cross_val_score(qsar_knn, qsar_X, qsar_y, cv=5)
        qsar_yvalues.append(cv_scores)
        if qsar_yvalues[-1] > qsar_last_best:
            qsar_best = (n, d)
            qsar_last_best = qsar_yvalues[-1]
    qsar_values[d] = qsar_yvalues

plt.figure()
ds.multiple_line_chart(qsar_nvalues, qsar_values, title='KNN variants', xlabel='n', ylabel='accuracy', percentage=True)
plt.show()
print('Best results with %d neighbors and %s'%(qsar_best[0], qsar_best[1]))

qsar_clf = qsar_knn = KNeighborsClassifier(n_neighbors = qsar_best[0], metric = qsar_best[1])
qsar_clf.fit(qsar_trnX, qsar_trnY)
qsar_prd_trn = qsar_clf.predict(qsar_trnX)
qsar_prd_tst = qsar_clf.predict(qsar_tstX)
ds.plot_evaluation_results(pd.unique(qsar_y), qsar_trnY, qsar_prd_trn, qsar_tstY, qsar_prd_tst)
plt.show()

#################################################################
######################Unsupervised Learning######################
#################################################################

###############
#Pattern Mining
###############

# Pattern Mining for the Heart Failure Clinical Records dataset

#this_hfp_data = pd.read_csv(hfp_path, sep=',', decimal='.', parse_dates=True, infer_datetime_format=True)
this_hfp_data = hfp_df
this_hfp_data.pop('DEATH_EVENT')
this_hfp_data = ds.dummify(this_hfp_data, this_hfp_data.columns)
this_hfp_data = this_hfp_data.astype("bool")
this_hfp_data.shape

#Patterns

MIN_SUP: float = 0.01
hfp_var_min_sup = [0.2, 0.1, 0.05, 0.01]

hfp_patterns: pd.DataFrame = pm.apriori(this_hfp_data, min_support = MIN_SUP, use_colnames = True, verbose = True)
print(len(hfp_patterns),'hfp_patterns')
hfp_nr_patterns = []
for sup in hfp_var_min_sup:
    hfp_pat = hfp_patterns[hfp_patterns['support']>=sup]
    hfp_nr_patterns.append(len(hfp_pat))

plt.figure(figsize = (6, 4))
ds.plot_line(hfp_var_min_sup, hfp_nr_patterns, title = 'Nr Patterns x Support', xlabel = 'support', ylabel = 'Nr Patterns')
plt.show()

#Association Rules

MIN_CONF: float = 0.1
hfp_rules = pm.association_rules(hfp_patterns, metric = 'confidence', min_threshold = MIN_CONF * 5, support_only = False)
print(f'\tfound {len(hfp_rules)} hfp_rules')

#Quality evaluation per support

hfp_nr_rules_sp = analyse_per_metric(hfp_rules, 'support', hfp_var_min_sup)
ds.plot_line(hfp_var_min_sup, hfp_nr_rules_sp, title = 'Nr rules x Support', xlabel = 'support', ylabel = 'Nr. rules', percentage = False)

#Quality evaluation per confidence

hfp_var_min_conf = [i * MIN_CONF for i in range(10, 5, -1)]
hfp_nr_rules_cf = analyse_per_metric(hfp_rules, 'confidence', hfp_var_min_conf)
ds.plot_line(hfp_var_min_conf, hfp_nr_rules_cf, title = 'Nr Rules x Confidence', xlabel = 'confidence', ylabel = 'Nr Rules', percentage = False)

#### Oral Toxicity dataset
"""
this_qsar_data = pd.read_csv(qsar_path, sep=';')
this_qsar_data = ds.dummify(this_qsar_data, this_qsar_data.columns)
this_qsar_data = this_qsar_data.astype("bool")
this_qsar_data.shape

#Patterns

MIN_SUP: float = 0.01
qsar_var_min_sup = [0.2, 0.1, 0.05, 0.01]

qsar_patterns: pd.DataFrame = pm.apriori(this_qsar_data, min_support = MIN_SUP, use_colnames = True, verbose = True)
print(len(qsar_patterns),'qsar_patterns')
qsar_nr_patterns = []
for sup in qsar_var_min_sup:
    qsar_pat = qsar_patterns[qsar_patterns['support']>=sup]
    qsar_nr_patterns.append(len(qsar_pat))

plt.figure(figsize = (6, 4))
ds.plot_line(qsar_var_min_sup, qsar_nr_patterns, title = 'Nr Patterns x Support', xlabel = 'support', ylabel = 'Nr Patterns')
plt.show()

#Association Rules

MIN_CONF: float = 0.1
qsar_rules = pm.association_rules(qsar_patterns, metric = 'confidence', min_threshold = MIN_CONF * 5, support_only = False)
print(f'\tfound {len(qsar_rules)} qsar_rules')

#Quality evaluation per support

qsar_nr_rules_sp = analyse_per_metric(qsar_rules, 'support', qsar_var_min_sup)
ds.plot_line(qsar_var_min_sup, qsar_nr_rules_sp, title = 'Nr rules x Support', xlabel = 'support', ylabel = 'Nr. rules', percentage = False)

#Quality evaluation per confidence

qsar_var_min_conf = [i * MIN_CONF for i in range(10, 5, -1)]
qsar_nr_rules_cf = analyse_per_metric(qsar_rules, 'confidence', qsar_var_min_conf)
ds.plot_line(qsar_var_min_conf, qsar_nr_rules_cf, title = 'Nr Rules x Confidence', xlabel = 'confidence', ylabel = 'Nr Rules', percentage = False)
"""
###########
#Clustering
###########

RANDOM_STATE = 42

#hfp_data = hfp_df_over
hfp_data = this_hfp_data
#hfp_data.pop('DEATH_EVENT')
numeric_vars_hfp = hfp_data.select_dtypes(include='number').columns

# Variables
v1 = 7
v2 = 8
print(hfp_data)
N_CLUSTERS = [2, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29]
rows, cols = ds.choose_grid(len(N_CLUSTERS))

# K-Means

mse: list = []
sc: list = []
fig, axs = plt.subplots(rows, cols, figsize=(cols*5, rows*5), squeeze = False)
i, j = 0, 0
for n in range(len(N_CLUSTERS)):
    k = N_CLUSTERS[n]
    estimator = KMeans(n_clusters = k, random_state = RANDOM_STATE)
    estimator.fit(hfp_data)
    mse.append(estimator.inertia_)
    sc.append(silhouette_score(hfp_data, estimator.labels_))
    ds.plot_clusters2(hfp_data, v2, v1, estimator.labels_.astype(float), estimator.cluster_centers_, k,
                     f'KMeans k={k}', ax = axs[i,j])
    i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
plt.show()

fig, ax = plt.subplots(1, 2, figsize = (6, 3), squeeze=False)
ds.plot_line(N_CLUSTERS, mse, title = 'KMeans MSE', xlabel='k', ylabel='MSE', ax=ax[0, 0])
ds.plot_line(N_CLUSTERS, sc, title = 'KMeans SC', xlabel='k', ylabel='SC', ax=ax[0, 1], percentage=True)
plt.show()

# EM (Expectation-Maximization)

mse: list = []
sc: list = []
_, axs = plt.subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
i, j = 0, 0
for n in range(len(N_CLUSTERS)):
    k = N_CLUSTERS[n]
    estimator = GaussianMixture(n_components=k)
    estimator.fit(hfp_data)
    labels = estimator.predict(hfp_data)
    mse.append(ds.compute_mse(hfp_data.values, labels, estimator.means_))
    sc.append(silhouette_score(hfp_data, labels))
    ds.plot_clusters2(hfp_data, v2, v1, labels.astype(float), estimator.means_, k,
                     f'EM k={k}', ax=axs[i,j])
    i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
plt.show()

fig, ax = plt.subplots(1, 2, figsize=(6, 3), squeeze=False)
ds.plot_line(N_CLUSTERS, mse, title='EM MSE', xlabel='k', ylabel='MSE', ax=ax[0, 0])
ds.plot_line(N_CLUSTERS, sc, title='EM SC', xlabel='k', ylabel='SC', ax=ax[0, 1], percentage=True)
plt.show()

# EPS - studying the maximum distance impact

EPS = [2.5, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
mse: list = []
sc: list = []
rows, cols = ds.choose_grid(len(EPS))
_, axs = plt.subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
i, j = 0, 0
for n in range(len(EPS)):
    estimator = DBSCAN(eps=EPS[n], min_samples=2)
    estimator.fit(hfp_data)
    labels = estimator.labels_
    k = len(set(labels)) - (1 if -1 in labels else 0)
    if k > 1:
        centers = ds.compute_centroids(hfp_data, labels)
        mse.append(ds.compute_mse(hfp_data.values, labels, centers))
        sc.append(silhouette_score(hfp_data, labels))
        ds.plot_clusters2(hfp_data, v2, v1, labels.astype(float), estimator.components_, k,
                         f'DBSCAN eps={EPS[n]} k={k}', ax=axs[i,j])
        i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
    else:
        mse.append(0)
        sc.append(0)
plt.show()

fig, ax = plt.subplots(1, 2, figsize=(6, 3), squeeze=False)
ds.plot_line(EPS, mse, title='DBSCAN MSE', xlabel='eps', ylabel='MSE', ax=ax[0, 0])
ds.plot_line(EPS, sc, title='DBSCAN SC', xlabel='eps', ylabel='SC', ax=ax[0, 1], percentage=True)
plt.show()

# Metric

METRICS = ['euclidean', 'cityblock', 'chebyshev', 'cosine', 'jaccard']
distances = []
for m in METRICS:
    dist = np.mean(np.mean(squareform(pdist(hfp_data.values, metric=m))))
    distances.append(dist)

print('AVG distances among records', distances)
distances[0] *= 0.6
distances[1] = 80
distances[2] *= 0.6
distances[3] *= 0.1
distances[4] *= 0.15
print('CHOSEN EPS', distances)

mse: list = []
sc: list = []
rows, cols = ds.choose_grid(len(METRICS))
_, axs = plt.subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
i, j = 0, 0
for n in range(len(METRICS)):
    estimator = DBSCAN(eps=distances[n], min_samples=2, metric=METRICS[n])
    estimator.fit(hfp_data)
    labels = estimator.labels_
    k = len(set(labels)) - (1 if -1 in labels else 0)
    if k > 1:
        centers = ds.compute_centroids(hfp_data, labels)
        mse.append(ds.compute_mse(hfp_data.values, labels, centers))
        sc.append(silhouette_score(hfp_data, labels))
        ds.plot_clusters2(hfp_data, v2, v1, labels.astype(float), estimator.components_, k,
                         f'DBSCAN metric={METRICS[n]} eps={distances[n]:.2f} k={k}', ax=axs[i,j])
    else:
        mse.append(0)
        sc.append(0)
    i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
plt.show()

fig, ax = plt.subplots(1, 2, figsize=(6, 3), squeeze=False)
ds.bar_chart(METRICS, mse, title='DBSCAN MSE', xlabel='metric', ylabel='MSE', ax=ax[0, 0])
ds.bar_chart(METRICS, sc, title='DBSCAN SC', xlabel='metric', ylabel='SC', ax=ax[0, 1], percentage=True)
plt.show()

# Hierarchical

mse: list = []
sc: list = []
rows, cols = ds.choose_grid(len(N_CLUSTERS))
_, axs = plt.subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
i, j = 0, 0
for n in range(len(N_CLUSTERS)):
    k = N_CLUSTERS[n]
    estimator = AgglomerativeClustering(n_clusters=k)
    estimator.fit(hfp_data)
    labels = estimator.labels_
    centers = ds.compute_centroids(hfp_data, labels)
    mse.append(ds.compute_mse(hfp_data.values, labels, centers))
    sc.append(silhouette_score(hfp_data, labels))
    ds.plot_clusters2(hfp_data, v2, v1, labels, centers, k,
                     f'Hierarchical k={k}', ax=axs[i,j])
    i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
plt.show()

fig, ax = plt.subplots(1, 2, figsize=(6, 3), squeeze=False)
ds.plot_line(N_CLUSTERS, mse, title='Hierarchical MSE', xlabel='k', ylabel='MSE', ax=ax[0, 0])
ds.plot_line(N_CLUSTERS, sc, title='Hierarchical SC', xlabel='k', ylabel='SC', ax=ax[0, 1], percentage=True)
plt.show()

METRICS = ['euclidean', 'cityblock', 'chebyshev', 'cosine', 'jaccard']
LINKS = ['complete', 'average']
k = 3
values_mse = {}
values_sc = {}
rows = len(METRICS)
cols = len(LINKS)
_, axs = plt.subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
for i in range(len(METRICS)):
    mse: list = []
    sc: list = []
    m = METRICS[i]
    for j in range(len(LINKS)):
        link = LINKS[j]
        estimator = AgglomerativeClustering(n_clusters=k, linkage=link, affinity=m )
        estimator.fit(hfp_data)
        labels = estimator.labels_
        centers = ds.compute_centroids(hfp_data, labels)
        mse.append(ds.compute_mse(hfp_data.values, labels, centers))
        sc.append(silhouette_score(hfp_data, labels))
        ds.plot_clusters2(hfp_data, v2, v1, labels, centers, k,
                         f'Hierarchical k={k} metric={m} link={link}', ax=axs[i,j])
    values_mse[m] = mse
    values_sc[m] = sc
plt.show()

_, ax = plt.subplots(1, 2, figsize=(6, 3), squeeze=False)
ds.multiple_bar_chart(LINKS, values_mse, title=f'Hierarchical MSE', xlabel='metric', ylabel='MSE', ax=ax[0, 0])
ds.multiple_bar_chart(LINKS, values_sc, title=f'Hierarchical SC', xlabel='metric', ylabel='SC', ax=ax[0, 1], percentage=True)
plt.show()


####################################
############Classification##########
####################################

####
#KNN
####

print("\n\n\n     printing KNN\n")

#KNN for Heart Failure Clinical Records dataset

hfp_data = hfp_df_over
#numeric_vars_hfp = hfp_data.select_dtypes(include='number').columns
labels = pd.unique(hfp_y)

hfp_trnX, hfp_tstX, hfp_trnY, hfp_tstY = train_test_split(hfp_X, hfp_y, train_size = 0.7, stratify = hfp_y)

hfp_nvalues = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
hfp_dist = ['manhattan', 'euclidean', 'chebyshev']
hfp_values = {}
hfp_best = (0, '')
hfp_last_best = 0

count = 0

for d in hfp_dist:
    hfp_yvalues = []
    for n in hfp_nvalues:
        hfp_knn = KNeighborsClassifier(n_neighbors = n, metric = d)
        hfp_knn.fit(hfp_trnX, hfp_trnY)
        hfp_prdY = hfp_knn.predict(hfp_tstX)
        hfp_yvalues.append(metrics.accuracy_score(hfp_tstY, hfp_prdY))
        if hfp_yvalues[-1] > hfp_last_best:
            hfp_best = (n, d)
            hfp_last_best = hfp_yvalues[-1]
    hfp_values[d] = hfp_yvalues

plt.figure()
ds.multiple_line_chart(hfp_nvalues, hfp_values, title='KNN variants', xlabel='n', ylabel='accuracy', percentage=True)
plt.show()
print('Best results with %d neighbors and %s'%(hfp_best[0], hfp_best[1]))

hfp_clf = hfp_knn = KNeighborsClassifier(n_neighbors = hfp_best[0], metric = hfp_best[1])
hfp_clf.fit(hfp_trnX, hfp_trnY)
hfp_prd_trn = hfp_clf.predict(hfp_trnX)
hfp_prd_tst = hfp_clf.predict(hfp_tstX)
ds.plot_evaluation_results(pd.unique(hfp_y), hfp_trnY, hfp_prd_trn, hfp_tstY, hfp_prd_tst)
plt.show()

#KNN for Oral Toxicity dataset

qsar_data = qsar_df_under
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns
labels = pd.unique(qsar_y)

qsar_trnX, qsar_tstX, qsar_trnY, qsar_tstY = train_test_split(qsar_X, qsar_y, train_size = 0.7, stratify = qsar_y)

qsar_nvalues = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19]
qsar_dist = ['manhattan', 'euclidean', 'chebyshev']
qsar_values = {}
qsar_best = (0, '')
qsar_last_best = 0
for d in qsar_dist:
    qsar_yvalues = []
    for n in qsar_nvalues:
        qsar_knn = KNeighborsClassifier(n_neighbors = n, metric = d)
        qsar_knn.fit(qsar_trnX, qsar_trnY)
        qsar_prdY = qsar_knn.predict(qsar_tstX)
        qsar_yvalues.append(metrics.accuracy_score(qsar_tstY, qsar_prdY))
        if qsar_yvalues[-1] > qsar_last_best:
            qsar_best = (n, d)
            qsar_last_best = qsar_yvalues[-1]
    qsar_values[d] = qsar_yvalues

plt.figure()
ds.multiple_line_chart(qsar_nvalues, qsar_values, title='KNN variants', xlabel='n', ylabel='accuracy', percentage=True)
plt.show()
print('Best results with %d neighbors and %s'%(qsar_best[0], qsar_best[1]))

qsar_clf = qsar_knn = KNeighborsClassifier(n_neighbors = qsar_best[0], metric = qsar_best[1])
qsar_clf.fit(qsar_trnX, qsar_trnY)
qsar_prd_trn = qsar_clf.predict(qsar_trnX)
qsar_prd_tst = qsar_clf.predict(qsar_tstX)
ds.plot_evaluation_results(pd.unique(qsar_y), qsar_trnY, qsar_prd_trn, qsar_tstY, qsar_prd_tst)
plt.show()

############
#Naive Bayes
############

print("\n\n\n     printing Naive Bayes\n")

#Naive Bayes for Heart Failure Clinical Records dataset

hfp_data = hfp_df_over

hfp_y: np.ndarray = hfp_data.pop('DEATH_EVENT').values
hfp_X: np.ndarray = hfp_data.values

hfp_labels = pd.unique(hfp_y)
hfp_trnX, hfp_tstX, hfp_trnY, hfp_tstY = train_test_split(hfp_X, hfp_y, train_size=0.7, stratify = hfp_y)
hfp_clf = GaussianNB()
hfp_clf.fit(hfp_trnX, hfp_trnY)
hfp_prd_trn = hfp_clf.predict(hfp_trnX)
hfp_prd_tst = hfp_clf.predict(hfp_tstX)
ds.plot_evaluation_results(pd.unique(hfp_y), hfp_trnY, hfp_prd_trn, hfp_tstY, hfp_prd_tst)

hfp_estimators = {'GaussianNB': GaussianNB(), 'MultinomialNB': MultinomialNB(), 'BernoulyNB': BernoulliNB()}
hfp_xvalues = []
hfp_yvalues = []

for clf in hfp_estimators:
    hfp_xvalues.append(clf)
    hfp_estimators[clf].fit(hfp_trnX, hfp_trnY)
    hfp_prdY = hfp_estimators[clf].predict(hfp_tstX)
    hfp_yvalues.append(metrics.accuracy_score(hfp_tstY, hfp_prdY))

plt.figure()
ds.bar_chart(hfp_xvalues, hfp_yvalues, title='Comparison of Naive Bayes Models', ylabel='accuracy', percentage=True)
plt.show()

#Naive Bayes for Oral Toxicity dataset

qsar_data = qsar_df_under

qsar_y: np.ndarray = qsar_data.pop('negative').values
qsar_X: np.ndarray = qsar_data.values

qsar_labels = pd.unique(qsar_y)
qsar_trnX, qsar_tstX, qsar_trnY, qsar_tstY = train_test_split(qsar_X, qsar_y, train_size=0.7, stratify = qsar_y)
qsar_clf = GaussianNB()
qsar_clf.fit(qsar_trnX, qsar_trnY)
qsar_prd_trn = qsar_clf.predict(qsar_trnX)
qsar_prd_tst = qsar_clf.predict(qsar_tstX)
ds.plot_evaluation_results(pd.unique(qsar_y), qsar_trnY, qsar_prd_trn, qsar_tstY, qsar_prd_tst)

qsar_estimators = {'GaussianNB': GaussianNB(), 'MultinomialNB': MultinomialNB(), 'BernoulyNB': BernoulliNB()}
qsar_xvalues = []
qsar_yvalues = []

for clf in qsar_estimators:
    qsar_xvalues.append(clf)
    qsar_estimators[clf].fit(qsar_trnX, qsar_trnY)
    qsar_prdY = qsar_estimators[clf].predict(qsar_tstX)
    qsar_yvalues.append(metrics.accuracy_score(qsar_tstY, qsar_prdY))

plt.figure()
ds.bar_chart(qsar_xvalues, qsar_yvalues, title='Comparison of Naive Bayes Models', ylabel='accuracy', percentage=True)
plt.show()

###############
#Decision Trees
###############

print("\n\n\n     printing Decision Trees\n")

#Decision Trees for the Heart Failure Clinical Records dataset

hfp_data = hfp_df_under
numeric_vars_hfp = hfp_data.select_dtypes(include='number').columns
labels = pd.unique(hfp_y)

hfp_trnX, hfp_tstX, hfp_trnY, hfp_tstY = train_test_split(hfp_X, hfp_y, train_size=0.7, stratify = hfp_y)

hfp_min_impurity_decrease = [0.025, 0.01, 0.005, 0.0025, 0.001]
hfp_max_depths = [2, 5, 10, 15, 20, 25]
hfp_criteria = ['entropy', 'gini']
hfp_best = ('',  0, 0.0)
hfp_last_best = 0
hfp_best_tree = None

plt.figure()
hfp_fig, hfp_axs = plt.subplots(1, 2, figsize=(16, 4), squeeze=False)
for k in range(len(hfp_criteria)):
    f = hfp_criteria[k]
    hfp_values = {}
    for d in hfp_max_depths:
        hfp_yvalues = []
        for imp in hfp_min_impurity_decrease:
            hfp_tree = DecisionTreeClassifier(min_samples_leaf = n, max_depth = d, criterion = f, min_impurity_decrease = imp)
            hfp_tree.fit(hfp_trnX, hfp_trnY)
            hfp_prdY = hfp_tree.predict(hfp_tstX)
            hfp_yvalues.append(metrics.accuracy_score(hfp_tstY, hfp_prdY))
            if hfp_yvalues[-1] > hfp_last_best:
                hfp_best = (f, d, imp)
                hfp_last_best = hfp_yvalues[-1]
                hfp_best_tree = hfp_tree

        hfp_values[d] = hfp_yvalues
    ds.multiple_line_chart(hfp_min_impurity_decrease, hfp_values, ax = hfp_axs[0, k], title='Decision Trees with %s criteria'%f,
                           xlabel='min_impurity_decrease', ylabel='accuracy', percentage=True)

plt.show()
print('Best results achieved with %s criteria, depth=%d and min_impurity_decrease=%1.2f ==> accuracy=%1.2f'%(hfp_best[0], hfp_best[1],
    hfp_best[2], hfp_last_best))

dot_data = export_graphviz(hfp_tree, out_file = 'dtree.dot', filled = True, rounded = True, special_characters = True)

# Convert to png

#call(['dot', '-Tpng', 'dtree.dot', '-o', 'dtree.png', '-Gdpi=600'])

plt.figure(figsize = (14, 18))
#plt.imshow(plt.imread('dtree.png'))
plt.axis('off')
#plt.show()

hfp_prd_trn = hfp_best_tree.predict(hfp_trnX)
hfp_prd_tst = hfp_best_tree.predict(hfp_tstX)
ds.plot_evaluation_results(pd.unique(hfp_y), hfp_trnY, hfp_prd_trn, hfp_tstY, hfp_prd_tst)
plt.show()

#Decision Trees for the Oral Toxicity dataset

qsar_data = qsar_df_under
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns

labels = pd.unique(qsar_y)

qsar_trnX, qsar_tstX, qsar_trnY, qsar_tstY = train_test_split(qsar_X, qsar_y, train_size=0.7, stratify = qsar_y)

qsar_min_impurity_decrease = [0.025, 0.01, 0.005, 0.0025, 0.001]
qsar_max_depths = [2, 5, 10, 15, 20, 25]
qsar_criteria = ['entropy', 'gini']
qsar_best = ('',  0, 0.0)
qsar_last_best = 0
qsar_best_tree = None

plt.figure()
qsar_fig, qsar_axs = plt.subplots(1, 2, figsize=(16, 4), squeeze=False)
for k in range(len(qsar_criteria)):
    f = qsar_criteria[k]
    qsar_values = {}
    for d in qsar_max_depths:
        qsar_yvalues = []
        for imp in qsar_min_impurity_decrease:
            qsar_tree = DecisionTreeClassifier(min_samples_leaf = n, max_depth = d, criterion = f, min_impurity_decrease = imp)
            qsar_tree.fit(qsar_trnX, qsar_trnY)
            qsar_prdY = qsar_tree.predict(qsar_tstX)
            qsar_yvalues.append(metrics.accuracy_score(qsar_tstY, qsar_prdY))
            if qsar_yvalues[-1] > qsar_last_best:
                qsar_best = (f, d, imp)
                qsar_last_best = qsar_yvalues[-1]
                qsar_best_tree = qsar_tree

        qsar_values[d] = qsar_yvalues
    ds.multiple_line_chart(qsar_min_impurity_decrease, qsar_values, ax = qsar_axs[0, k], title='Decision Trees with %s criteria'%f,
                           xlabel='min_impurity_decrease', ylabel='accuracy', percentage=True)

plt.show()
print('Best results achieved with %s criteria, depth=%d and min_impurity_decrease=%1.2f ==> accuracy=%1.2f'%(qsar_best[0], qsar_best[1],
    qsar_best[2], qsar_last_best))

dot_data = export_graphviz(qsar_tree, out_file = 'dtree.dot', filled = True, rounded = True, special_characters = True)

# Convert to png

#call(['dot', '-Tpng', 'dtree.dot', '-o', 'dtree.png', '-Gdpi=600'])

plt.figure(figsize = (14, 18))
#plt.imshow(plt.imread('dtree.png'))
plt.axis('off')
plt.show()

qsar_prd_trn = qsar_best_tree.predict(qsar_trnX)
qsar_prd_tst = qsar_best_tree.predict(qsar_tstX)
ds.plot_evaluation_results(pd.unique(qsar_y), qsar_trnY, qsar_prd_trn, qsar_tstY, qsar_prd_tst)
plt.show()

###############
#Random forests
###############

#Random forests for the Heart Failure Clinical Records dataset
#Está tudo comentado, pois demoram muito a correr, mas estão bem.

hfp_data = hfp_df
hfp_y = hfp_data.pop('DEATH_EVENT').values
hfp_X = hfp_data.values
labels = pd.unique(hfp_y)

hfp_trnX, hfp_tstX, hfp_trnY, hfp_tstY = train_test_split(hfp_X, hfp_y, train_size = 0.7, stratify = hfp_y)

hfp_n_estimators = [5, 10, 25, 50, 75, 100, 150, 200, 250, 300]
hfp_max_depths = [5, 10, 25]
hfp_max_features = [.1, .3, .5, .7, .9, 1]
hfp_best = ('', 0, 0)
hfp_last_best = 0
hfp_best_tree = None

hfp_cols = len(hfp_max_depths)
plt.figure()
hfp_fig, hfp_axs = plt.subplots(1, hfp_cols, figsize = (hfp_cols*ds.HEIGHT, ds.HEIGHT), squeeze = False)
for k in range(len(hfp_max_depths)):
    d = hfp_max_depths[k]
    hfp_values = {}
    for f in hfp_max_features:
        hfp_yvalues = []
        for n in hfp_n_estimators:
            hfp_rf = RandomForestClassifier(n_estimators = n, max_depth = d, max_features = f)
            hfp_rf.fit(hfp_trnX, hfp_trnY)
            hfp_prdY = hfp_rf.predict(hfp_tstX)
            hfp_yvalues.append(metrics.accuracy_score(hfp_tstY, hfp_prdY))
            if hfp_yvalues[-1] > hfp_last_best:
                hfp_best = (d, f, n)
                hfp_last_best = hfp_yvalues[-1]
                hfp_best_tree = hfp_rf

        hfp_values[f] = hfp_yvalues
    ds.multiple_line_chart(hfp_n_estimators, hfp_values, ax = hfp_axs[0, k],
                            title = 'Random Forests with max_depth=%d'%d,
                            xlabel='nr estimators', ylabel='accuracy', percentage=True)

plt.show()
print('Best results with depth=%d, %1.2f features and %d estimators, with accuracy=%1.2f'%(hfp_best[0], hfp_best[1], hfp_best[2], hfp_last_best))

hfp_prd_trn = hfp_best_tree.predict(hfp_trnX)
hfp_prd_tst = hfp_best_tree.predict(hfp_tstX)
ds.plot_evaluation_results(pd.unique(hfp_y), hfp_trnY, hfp_prd_trn, hfp_tstY, hfp_prd_tst)
plt.show()

#Random forests for the Oral Toxicity dataset

qsar_data = qsar_dataFrame
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns
qsar_y = qsar_data.pop('negative').values
qsar_X = qsar_data.values

labels = pd.unique(qsar_y)

qsar_trnX, qsar_tstX, qsar_trnY, qsar_tstY = train_test_split(qsar_X, qsar_y, train_size = 0.7, stratify = qsar_y)

qsar_n_estimators = [5, 10, 25, 50, 75, 100, 150, 200, 250, 300]
qsar_max_depths = [5, 10, 25]
qsar_max_features = [.1, .3, .5, .7, .9, 1]
qsar_best = ('', 0, 0)
qsar_last_best = 0
qsar_best_tree = None

qsar_cols = len(qsar_max_depths)
plt.figure()
qsar_fig, qsar_axs = plt.subplots(1, qsar_cols, figsize = (qsar_cols*ds.HEIGHT, ds.HEIGHT), squeeze = False)
for k in range(len(qsar_max_depths)):
    d = qsar_max_depths[k]
    qsar_values = {}
    for f in qsar_max_features:
        qsar_yvalues = []
        for n in qsar_n_estimators:
            qsar_rf = RandomForestClassifier(n_estimators = n, max_depth = d, max_features = f)
            qsar_rf.fit(qsar_trnX, qsar_trnY)
            qsar_prdY = qsar_rf.predict(qsar_tstX)
            qsar_yvalues.append(metrics.accuracy_score(qsar_tstY, qsar_prdY))
            if qsar_yvalues[-1] > qsar_last_best:
                qsar_best = (d, f, n)
                qsar_last_best = qsar_yvalues[-1]
                qsar_best_tree = qsar_rf

        qsar_values[f] = qsar_yvalues
    ds.multiple_line_chart(qsar_n_estimators, qsar_values, ax = qsar_axs[0, k],
                            title = 'Random Forests with max_depth=%d'%d,
                            xlabel = 'nr estimators', ylabel = 'accuracy', percentage = True)

plt.show()
print('Best results with depth=%d, %1.2f features and %d estimators, with accuracy=%1.2f'%(qsar_best[0], qsar_best[1], qsar_best[2], qsar_last_best))

qsar_prd_trn = qsar_best_tree.predict(qsar_trnX)
qsar_prd_tst = qsar_best_tree.predict(qsar_tstX)
ds.plot_evaluation_results(pd.unique(qsar_y), qsar_trnY, qsar_prd_trn, qsar_tstY, qsar_prd_tst)
plt.show()

##################
#Gradient Boosting
##################

#Gradient Boosting for the Heart Failure Clinical Records dataset

hfp_data = hfp_df_under
numeric_vars_hfp = hfp_data.select_dtypes(include='number').columns
hfp_y: np.ndarray = hfp_data.pop('DEATH_EVENT').values
hfp_X: np.ndarray = hfp_data.values
hfp_labels = pd.unique(hfp_y)

hfp_trnX, hfp_tstX, hfp_trnY, hfp_tstY = train_test_split(hfp_X, hfp_y, train_size = 0.7, stratify = hfp_y)

hfp_n_estimators = [5, 10, 25, 50, 75, 100, 150, 200, 250, 300]
hfp_max_depths = [5, 10, 25]
hfp_learning_rate = [.1, .3, .5, .7, .9]
hfp_best = ('', 0, 0)
hfp_last_best = 0
hfp_best_tree = None

hfp_cols = len(hfp_max_depths)
plt.figure()
hfp_fig, hfp_axs = plt.subplots(1, hfp_cols, figsize = (hfp_cols*ds.HEIGHT, ds.HEIGHT), squeeze = False)
for k in range(len(hfp_max_depths)):
    d = hfp_max_depths[k]
    hfp_values = {}
    for lr in hfp_learning_rate:
        hfp_yvalues = []
        for n in hfp_n_estimators:
            hfp_gb = GradientBoostingClassifier(n_estimators = n, max_depth = d, learning_rate = lr)
            hfp_gb.fit(hfp_trnX, hfp_trnY)
            hfp_prdY = hfp_gb.predict(hfp_tstX)
            hfp_yvalues.append(metrics.accuracy_score(hfp_tstY, hfp_prdY))
            if hfp_yvalues[-1] > hfp_last_best:
                hfp_best = (d, lr, n)
                hfp_last_best = hfp_yvalues[-1]
                hfp_best_tree = hfp_gb
        hfp_values[lr] = hfp_yvalues
    ds.multiple_line_chart(hfp_n_estimators, hfp_values, ax = hfp_axs[0, k], 
                           title='Gradient Boorsting with max_depth=%d'%d,
                           xlabel='nr estimators', ylabel='accuracy', percentage=True)

plt.show()
print('Best results for the Heart Failure Clinical Records dataset with depth=%d, learning rate=%1.2f and %d estimators, with accuracy=%1.2f'%(
    hfp_best[0], hfp_best[1], hfp_best[2], hfp_last_best))

#Gradient Boosting for the Oral Toxicity dataset

qsar_data = qsar_df_under
numeric_vars_qsar = qsar_data.select_dtypes(include='number').columns

qsar_df_nr = pd.DataFrame(qsar_imp_nr.fit_transform(qsar_cols_nr), columns=qsar_cols_nr.columns)
qsar_df_sb = pd.DataFrame(qsar_imp_sb.fit_transform(qsar_cols_sb), columns=qsar_cols_sb.columns)
qsar_dataFrame = qsar_df_nr.join(qsar_df_sb, how='right')
qsar_dataFrame.describe(include='all')

qsar_y: np.ndarray = qsar_df_nr.pop('0.970').values
qsar_X: np.ndarray = qsar_df_nr.values
qsar_labels = pd.unique(qsar_y)

qsar_trnX, qsar_tstX, qsar_trnY, qsar_tstY = train_test_split(qsar_X, qsar_y, train_size = 0.7, stratify = qsar_y)

qsar_n_estimators = [5, 10, 25, 50, 75, 100, 150, 200, 250, 300]
qsar_max_depths = [5, 10, 25]
qsar_learning_rate = [.1, .3, .5, .7, .9]
qsar_best = ('', 0, 0)
qsar_last_best = 0
qsar_best_tree = None

qsar_cols = len(qsar_max_depths)
plt.figure()
qsar_fig, qsar_axs = plt.subplots(1, qsar_cols, figsize = (qsar_cols*ds.HEIGHT, ds.HEIGHT), squeeze = False)
for k in range(len(qsar_max_depths)):
    d = qsar_max_depths[k]
    qsar_values = {}
    for lr in qsar_learning_rate:
        qsar_yvalues = []
        for n in qsar_n_estimators:
            qsar_gb = GradientBoostingClassifier(n_estimators = n, max_depth = d, learning_rate = lr)
            qsar_gb.fit(qsar_trnX, qsar_trnY)
            qsar_prdY = qsar_gb.predict(qsar_tstX)
            qsar_yvalues.append(metrics.accuracy_score(qsar_tstY, qsar_prdY))
            if qsar_yvalues[-1] > qsar_last_best:
                qsar_best = (d, lr, n)
                qsar_last_best = qsar_yvalues[-1]
                qsar_best_tree = qsar_gb
        qsar_values[lr] = qsar_yvalues
    ds.multiple_line_chart(qsar_n_estimators, qsar_values, ax = qsar_axs[0, k], 
                           title='Gradient Boorsting with max_depth=%d'%d,
                           xlabel='nr estimators', ylabel='accuracy', percentage=True)

plt.show()
print('Best results for the Heart Failure Clinical Records dataset with depth=%d, learning rate=%1.2f and %d estimators, with accuracy=%1.2f'%(
    qsar_best[0], qsar_best[1], qsar_best[2], qsar_last_best))

############
#Overfitting
############

#Olhar para as barras de accuracy do gráfico.











