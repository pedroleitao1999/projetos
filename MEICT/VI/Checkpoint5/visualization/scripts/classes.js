class Choropleth {
    constructor(filter, salePrice, rentPrice, criminality, population, purchasePower) {
        this.colorscaleSalePrice = salePrice;
        this.colorScaleRentPrice = rentPrice;
        this.securityRateScale = criminality;
        this.populationDensityScale = population;
        this.purchasePowerScale = purchasePower;
        this.selectedFilter = filter; // Starting will be rent price
        this.globalSelectedFilter = filter;
    }

    getActiveScale(d) {
        switch (this.selectedFilter) {
            case "Sell":
                return this.colorscaleSalePrice(d.properties.AverageSellPrice);
            case "Rent":
                return this.colorScaleRentPrice(d.properties.AverageRentPrice);
            case "Security":
                return this.securityRateScale(d.properties.Security);
            case "Population":
                return this.populationDensityScale(d.properties.PopulationDensity);
            case "PurchasePower":
                return this.purchasePowerScale(d.properties.PurchasePower);
        }
    }

    changeScale(attr, global) {
        if (global) {
            this.globalSelectedFilter = attr
            this.selectedFilter = this.globalSelectedFilter
            switch (attr) {
                case "Sell":
                    d3.select('#colorscale_choropleth_sale').select("svg").attr("visibility", "visible")
                    break;
                case "Rent":
                    d3.select('#colorscale_choropleth_rent').select("svg").attr("visibility", "visible")
                    break;
            }
        }
        else if (this.selectedFilter === attr) { //TURNING OFF THE FILTER TO GET THE GLOBAL ONE
            this.selectedFilter = this.globalSelectedFilter;
            switch (this.selectedFilter) {
                case "Sell":
                    d3.select('#colorscale_choropleth_sale').select("svg").attr("visibility", "visible")
                    break;
                case "Rent":
                    d3.select('#colorscale_choropleth_rent').select("svg").attr("visibility", "visible")
                    break;
            }
        } else {
            switch (attr) {
                case "Security":
                    d3.select('#colorscale_choropleth_security').select("svg").attr("visibility", "visible")
                    break;
                case "Population":
                    d3.select('#colorscale_choropleth_population').select("svg").attr("visibility", "visible")
                    break;
                case "PurchasePower":
                    d3.select('#colorscale_choropleth_purchase_power').select("svg").attr("visibility", "visible")
                    break;
            }
            this.selectedFilter = attr
        }
        d3.select("#choropleth").selectAll("path").attr("fill", (d) => this.getActiveScale(d)).attr("stroke", "white").attr("stroke-width", 1)
        viz.selectedDistricts = []
    }
}

class LineChart {
    constructor(sellColor, rentColor, selectedFilter) {
        this.sellColor = sellColor;
        this.rentColor = rentColor;
        this.selectedFilter = selectedFilter;
    }

    getColor() {
        switch (this.selectedFilter) {
            case "Sell":
                return this.sellColor;
            case "Rent":
                return this.rentColor;
        }
    }

    getSelectedColors() {
        return this.selectedColors;
    }
}

class RadarChart {
    constructor() {
    }
}

class Histogram {
    constructor() {

    }
}

class Datasets {
    constructor() {
        this.districts = []
    }
}

class Visualization {
    constructor() {
        this.activeFilter = "Sell";
        this.globalSelectedFilter = "Sell"
        this.selectedDistricts = []
        this.choropleth = null;
        this.datasets = new Datasets();
    }

    setActiveFilter(filter) {
        this.activeFilter = filter;
        this.choropleth.selectedFilter = filter;
    }
}
