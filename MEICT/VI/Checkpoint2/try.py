import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import re

#Auxiliar functions
def print_head_data_frames():
    print(ads_df.head)
    print(crimes_df.head)
    print(purchasePower_df.head)
    print(population_df.head)
    print(area_df.head)

def outlier_detection(datacolumn):
    sorted(datacolumn)
    Q1,Q3 = np.percentile(datacolumn , [25,75])
    IQR = Q3-Q1
    lower_range = Q1-(1.5 * IQR)
    upper_range = Q3+(1.5 * IQR)
    return lower_range,upper_range

def outlier_treatment(dataset, datacolumn):
    lowerbound,upperbound = outlier_detection(datacolumn)
    print(np.size(dataset[(datacolumn<lowerbound)|(datacolumn>upperbound)]))
    dataset.drop(dataset[(datacolumn>upperbound)|(datacolumn<lowerbound)].index , inplace=True)

def remove_trailling_space(file_name):
    lines  = []
    newline = ""
    with open(file_name, "r", encoding = 'utf-8') as fin:
        lines = fin.readlines()
    for i in range(0, len(lines)):
        lines[i] = lines[i].rstrip()
        lines[i] = newline + re.sub(r";$", "", lines[i])
        newline = "\n"

    with open(file_name, "w", encoding='utf-8') as fout:
        fout.write("".join(lines))

def calculate_average(data):
    distritos = ["Viana do Castelo", "Braga", "Porto", "Vila Real", "Bragança", "Viseu", "Guarda", "Aveiro", "Coimbra", "Castelo Branco", "Leiria", "Santarém", "Portalegre", "Lisboa", "Setúbal", "Évora", "Beja", "Faro"]

    for i, row in data.iterrows():
        for i in distritos:
            print(row[i])

# Clearing out trailling spaces
files = ["datasets/portugal_criminality_rates.csv", "datasets/purchase_power.csv", "datasets/population.csv", "datasets/portugal_area.csv"]
for file in files:
    remove_trailling_space(file)

#Import datasets
ads_df = pd.read_csv("datasets/portugal_ads_proprieties.csv", delimiter=',')
crimes_df = pd.read_csv("datasets/portugal_criminality_rates.csv", delimiter=';', encoding='utf-8')
purchasePower_df = pd.read_csv("datasets/purchase_power.csv", delimiter=';', encoding='utf-8')
population_df = pd.read_csv("datasets/population.csv", delimiter=';', encoding='utf-8')
area_df = pd.read_csv("datasets/portugal_area.csv", delimiter=';', encoding='utf-8')

#Drop irrelevant columns
crimes_df.drop(['CrimesAgainstPhysicalIntegrity', 'TheftOrRobbery', 'Carjacking', 'DrunkDriving', 'DrivingWithoutLicense',
    'CrimesAgainstNationalHeritage'], axis=1, inplace=True)
population_df.drop(['H', 'M'], axis=1, inplace=True)

#Drop missing values
old_size = np.size(ads_df)
ads_df.dropna(subset=["Locations", "Rooms", "Price", "PropertyArea", "Bathrooms", "Condition", "AdsType", "ProprietyType"], inplace=True)

old_size = np.size(crimes_df)
crimes_df.dropna(subset=["Location", "CrimeRate"], inplace=True)

old_size = np.size(purchasePower_df)
purchasePower_df.dropna(subset=["Location", "PurchasePower"], inplace=True)

old_size = np.size(population_df)
population_df.dropna(subset=["Location", "HM"], inplace=True)

old_size = np.size(area_df)
area_df.dropna(subset=["Location", "Area"], inplace=True)
area_df.sort_values("Location", inplace=True)
population_df.sort_values("Location", inplace=True)


#Outliers detection and removal
outlier_treatment(ads_df, ads_df['Price']) #FIXME treat outliers
outlier_treatment(ads_df, ads_df['PropertyArea'])


#Sort and remove duplicates
ads_df.sort_values("Locations", inplace=True)

# Extract the Locations
population_df["Locations"] = population_df['Location'].str.extract(r'([^0-9:]+$)', expand=False)
area_df["Locations"] = area_df['Location'].str.extract(r'([^0-9:]+$)', expand=False)
crimes_df["Locations"] = crimes_df['Location'].str.extract(r'([^0-9:]+$)', expand=False)
purchasePower_df["Locations"] = purchasePower_df['Location'].str.extract(r'([^0-9:]+$)', expand=False)
ads_df["Location_calculated"] = ads_df['Locations'].str.extract(r'(^[a-zA-Z|á|à|â|ã|é|è|ê|í|ì|î|ó|ò|õ|ô|ú|ù|û|Ç||á|à|â|ã|é|è|ê|í|ì|î|ó|ò|õ|ô|ú|ù|û|ç|Á|À|Â|Ã|É|È|Ê|Í|Ì|Î|Ó|Ò|Õ|Ô|Ú|Ù|Û|Ç| ]{3,})', expand=False)
ads_df["District"] = ads_df['Locations'].str.extract(r'([a-zA-Z|á|à|â|ã|é|è|ê|í|ì|î|ó|ò|õ|ô|ú|ù|û|ç|Á|À|Â|Ã|É|È|Ê|Í|Ì|Î|Ó|Ò|Õ|Ô|Ú|Ù|Û|Ç| ]{3,}$)',expand=False)

# Dropping unparsed location stuff
population_df.drop(["Location"], inplace=True, axis=1)
area_df.drop(["Location"], inplace=True, axis=1)
crimes_df.drop(["Location"], inplace=True, axis=1)
purchasePower_df.drop(["Location"], inplace=True, axis=1)

# Merge datasets to make information about a certain area (population, crimes, purchase power, etc.)
population_area_df = population_df.merge(area_df, left_on='Locations', right_on='Locations')
population_area_df = population_area_df.merge(crimes_df, left_on='Locations', right_on='Locations')
population_area_df = population_area_df.merge(purchasePower_df, how="outer", left_on='Locations', right_on='Locations')

#TODO BRAGANÇA, SANTARÉM, ETC a ficar perdidos

# Calculating the derived Measures (Population Density and TotalCrimes)
population_area_df['PopulationDensity'] = population_area_df["HM"] / population_area_df["Area"]
population_area_df['TotalCrimes'] = population_area_df["CrimeRate"] * population_area_df["HM"] / 1000

population_area_df.sort_values("PopulationDensity", inplace=True)
population_area_df.drop_duplicates(subset="Locations", keep='first', inplace=True)

ads_df["frequency"] = ads_df["Location_calculated"].map(ads_df["Location_calculated"].value_counts())

ads_df.sort_values("frequency", ascending=False).drop_duplicates("Locations").drop(["frequency"], axis=1, inplace=True)

# Select the Sell and Rent
ads_grouped_sell = ads_df[ads_df['AdsType'] == "Sell"].groupby(by=["Location_calculated", "ProprietyType", "Rooms", "Bathrooms", "District"])
ads_rent_mean_df = ads_df[ads_df['AdsType'] == "Rent"].groupby(by=["Location_calculated", "ProprietyType", "Rooms", "Bathrooms", "District"])

# Calculate the average Price for selling and renting
ads_grouped_sell = ads_grouped_sell['Price'].mean()
ads_rent_mean_df = ads_rent_mean_df['Price'].mean()

#TODO create for districts and parishes
population_area_ads_df_old = population_area_df.merge(ads_grouped_sell, how='outer', left_on='Locations', right_on='Location_calculated')

population_area_ads_df = population_area_ads_df_old.merge(ads_rent_mean_df, how='outer', left_on='Locations', right_on='Location_calculated')
population_area_ads_df.sort_values("PopulationDensity", inplace=True, ascending=False)

population_area_ads_df.drop_duplicates(subset="Locations", keep='first', inplace=True)
population_area_ads_df = population_area_ads_df.rename(columns={'Price_x': 'AverageSellPrice'})
population_area_ads_df = population_area_ads_df.rename(columns={'Price_y': 'AverageRentPrice'})

population_area_ads_df.drop(['HM', 'Area'], axis=1, inplace=True)

# ________________________________________________________________________________________________________________

ads_grouped_sell = ads_df[ads_df['AdsType'] == "Sell"].groupby(by=["District", "ProprietyType", "Rooms", "Bathrooms"])
ads_rent_mean_df = ads_df[ads_df['AdsType'] == "Rent"].groupby(by=["District", "ProprietyType",  "Rooms", "Bathrooms"])

ads_grouped_sell = ads_grouped_sell['Price'].mean()
ads_rent_mean_df = ads_rent_mean_df['Price'].mean()


district_merged = ads_grouped_sell.to_frame().merge(ads_rent_mean_df.to_frame(), on=["Rooms", "Bathrooms", "ProprietyType", "District"])

district_merged = district_merged.rename(columns={'Price_y': 'AverageRentPrice', 'Price_x': 'AverageSellPrice'})

district_merged.to_csv('district_median_price_for_each_type_of_house.csv', index=True, header=True)

# ________________________________________________________________________________________________________________

ads_grouped_sell = ads_df[ads_df['AdsType'] == "Sell"].groupby(by=["District", "ProprietyType"])
ads_rent_mean_df = ads_df[ads_df['AdsType'] == "Rent"].groupby(by=["District", "ProprietyType"])

rooms_sell = ads_grouped_sell['Rooms'].mean()
rooms_rent = ads_rent_mean_df['Rooms'].mean()

brooms_sell = ads_grouped_sell['Bathrooms'].mean()
brooms_rent = ads_rent_mean_df['Bathrooms'].mean()

sell_median = ads_grouped_sell['Price'].mean()
rent_median = ads_rent_mean_df['Price'].mean()

area_median = ads_rent_mean_df['PropertyArea'].mean()


sell_merged = sell_median.to_frame().merge(rent_median.to_frame(), on=["ProprietyType", "District"])
sell_merged = sell_merged.rename(columns={'Price_x': 'AverageSellPrice', 'Price_y': 'AverageRentPrice'})

rooms_merged = rooms_sell.to_frame().merge(rooms_rent.to_frame(), on=["ProprietyType", "District"])
rooms_merged = rooms_merged.rename(columns={'Rooms_x': 'AverageSellRooms', 'Rooms_y': 'AverageRentRooms'})


brooms_merged = brooms_sell.to_frame().merge(brooms_rent.to_frame(), on=["ProprietyType", "District"])
brooms_merged = brooms_merged.rename(columns={'Bathrooms_x': 'AverageSellBathRooms', 'Bathrooms_y': 'AverageRentBathRooms'})

merged = brooms_merged.merge(rooms_merged, on=["ProprietyType", "District"])
merged = merged.merge(sell_merged, on=["ProprietyType", "District"])

merged = merged.merge(area_median, on=["ProprietyType", "District"])

merged.to_csv('median_informations_district.csv', index=True, header=True)

# ________________________________________________________________________________________________________________

distritos = [" Viana do Castelo", " Braga", " Porto", " Vila Real", " Bragança", " Viseu", " Guarda", " Aveiro", " Coimbra", " Castelo Branco", " Leiria",
 " Santarém", " Portalegre", " Lisboa", " Setúbal", " Évora", " Beja", " Faro", "Ilha de São Miguel", "Ilha da Madeira"]

locations_districts = population_area_df.loc[population_area_df['Locations'].isin(distritos)] 

print(locations_districts.head())

locations_districts.to_csv('locations_districts.csv', index=False, header=True) #TODO filter to districts only


#Export as a csv
ads_df.to_csv('portuguese_real_estate_market.csv', index=True, header=True)
population_area_ads_df.to_csv('locations_parishes_info.csv', index=False, header=True)
