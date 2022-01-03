let lineChartX = "Density";
let lineChartY = "Sell";
const purchasePowerPurple = "#756bb1"
const securityColor = "#31a354"
const populationDensityOrange = "#e6550d"
const sellPriceGreen = "#ef3b2c"
const rentPriceBlue = "#3164a3"

const adsDataset = "data/portuguese_real_estate_market.csv"

let sellPriceMax
let rentPriceMax = 4000
let rentDomain = []
let sellDomain = []

let brushSelected = []

let bathsMax
let roomsMax
let areaMax
let bathsSortedData = []
let roomsSortedData = []
let areaSortedData = []

let locationsDistricts = []
let locationsInfo = []
let locationsParishesInfo = []

let districtLines = ["", "", "", "", ""]
let linesColors = ["#2eff00", "#e7298a", "#6a3d9a", "#d95f02", "#b15928"]
let dataSelectedDistricts = [[], [], [], [], []]

let scaleSecurity;
let viz = null;
function init() {
    viz = new Visualization()
    Promise.all([d3.json("data/portugal_districts.json"), d3.csv("data/locations_districts.csv")]).then(([data, locations]) => {
        viz.datasets.districts = locations
        createChoropleth(data, false);
        createChoroplethScales();
    });

    d3.csv(adsDataset)
        .then((data) => {
            viz.histogram = new Histogram();
            createHistogram(data, false);

            bathsSortedData = data.sort((a, b) =>
                d3.ascending(parseFloat(a.Bathrooms), parseFloat(b.Bathrooms)))
            roomsSortedData = data.sort((a, b) =>
                d3.ascending(parseFloat(a.Rooms), parseFloat(b.Rooms)))
            areaSortedData = data.sort((a, b) =>
                d3.ascending(parseFloat(a.PropertyArea), parseFloat(b.PropertyArea)))
            createBoxPlots(false)
        }).catch((error) => {
        console.log(error);
    });

    d3.csv("data/locations_districts.csv")
        .then((data) => {
            locationsDistricts = data;
            createRadarChart(locationsDistricts, false);
        }).catch((error) => {
            console.log(error);
    });

    d3.csv("data/parishes.csv").then((data) => {
        locationsParishesInfo = data;
        createLineChart(locationsParishesInfo, false);
    }).catch((error) => {
        console.log(error);
    });
}

function createChoroplethScales() {
    scales = ['colorscale_choropleth_security', 'colorscale_choropleth_population', 'colorscale_choropleth_purchase_power', 'colorscale_choropleth_sale', 'colorscale_choropleth_rent']
    colorscales = [viz.choropleth.securityRateScale, viz.choropleth.populationDensityScale, viz.choropleth.purchasePowerScale, viz.choropleth.colorscaleSalePrice, viz.choropleth.colorScaleRentPrice]

    maxs = [d3.max(viz.datasets.districts, (d) => parseFloat(d.Security)),
        d3.max(viz.datasets.districts, (d) => parseFloat(d.PopulationDensity)),
        d3.max(viz.datasets.districts, (d) => parseFloat(d.PurchasePower)),
        d3.max(viz.datasets.districts, (d) => parseFloat(d.AverageSellPrice)),
        d3.max(viz.datasets.districts, (d) => parseFloat(d.AverageRentPrice))]

    for (let i = 1; i < scales.length; i++) {
        var svgLegend = d3.select('#' + scales[i]).append('svg').attr("width", 60).attr("height", 350);
        var defs = svgLegend.append('defs');
        var linearGradient = defs.append('linearGradient').attr('id', 'linear-gradient-' + scales[i])
        linearGradient
            .attr("x1", "0%")
            .attr("y1", "100%")
            .attr("x2", "0%")
            .attr("y2", "0%");

        linearGradient.selectAll("stop")
            .data([
                {offset: "0%", color: colorscales[i](0)},
                {offset: "50%", color: colorscales[i](maxs[i]/3)},
                {offset: "100%", color: colorscales[i](maxs[i])}
            ]).enter().append("stop")
            .attr("offset", function(d) {
                return d.offset;
            })
            .attr("stop-color", function(d) {
                return d.color;
            });

        svgLegend.append("rect")
            .attr("x", 0)
            .attr("y", 30)
            .attr("width", 20)
            .attr("height", 300)
            .style("fill", "url(#linear-gradient-" + scales[i] + ")");

        //create tick marks
        var xLeg = d3.scaleLinear()
            .domain([0, numberFormatter(maxs[i]) * 10])
            .range([290, 0]);

        var axisLeg = d3.axisRight(xLeg).tickFormat(d3.format("d"));

        svgLegend
            .attr("class", "axis")
            .append("g")
            .attr("transform", "translate(20, 31)")
            .call(axisLeg);

        svgLegend.attr("visibility", "hidden")
    }

    var svgLegend = d3.select('#' + scales[0]).append('svg').attr("width", 60).attr("height", 350);
    var defs = svgLegend.append('defs');
    var linearGradient = defs.append('linearGradient').attr('id', 'linear-gradient-' + scales[0])
    linearGradient
        .attr("x1", "0%")
        .attr("y1", "100%")
        .attr("x2", "0%")
        .attr("y2", "0%");

    linearGradient.selectAll("stop")
        .data([
            {offset: "0%", color: scaleSecurity(0)},
            {offset: "50%", color: scaleSecurity(maxs[0]/4)},
            {offset: "90%", color: scaleSecurity(maxs[0]/4)},
            {offset: "100%", color: scaleSecurity(maxs[0])}
        ]).enter().append("stop")
        .attr("offset", function(d) {
            return d.offset;
        })
        .attr("stop-color", function(d) {
            return d.color;
        });

    svgLegend.append("rect")
        .attr("x", 0)
        .attr("y", 30)
        .attr("width", 20)
        .attr("height", 300)
        .style("fill", "url(#linear-gradient-" + scales[0] + ")");

    //create tick marks
    var xLeg = d3.scaleLinear()
        .domain([0, numberFormatter(maxs[0]) * 10])
        .range([290, 0]);

    var axisLeg = d3.axisRight(xLeg).tickFormat(d3.format("d"));

    svgLegend
        .attr("class", "axis")
        .append("g")
        .attr("transform", "translate(20, 31)")
        .call(axisLeg);

    svgLegend.attr("visibility", "hidden")
    d3.select('#colorscale_choropleth_sale').select("svg").attr("visibility", "visible")

}

function createChoropleth(data, update) {
    let width = 500;
    let height = 700;
    var projection = d3
        .geoMercator()
        .scale(6000)
        .rotate([0, 0])
        .center([-8, 39.6])
        .translate([width / 2, height / 2]);

    var path = d3.geoPath().projection(projection);

    let scalePopulationDensity = d3.scaleLinear().domain([0, d3.max(viz.datasets.districts, (d) => parseFloat(d.PopulationDensity))]).range(["#fee6ce", populationDensityOrange]);
    let scaleRentPrice = d3.scaleLinear().domain([0, d3.max(viz.datasets.districts, (d) => parseFloat(d.AverageRentPrice))]).range(["white", rentPriceBlue]);
    let scaleSellPrice = d3.scaleLinear().domain([0, d3.max(viz.datasets.districts, (d) => parseFloat(d.AverageSellPrice))]).range(["white", sellPriceGreen]);
    scaleSecurity = d3.scaleLinear().domain([0, d3.max(viz.datasets.districts, (d) => parseFloat(d.Security))]).range(["white", securityColor]);
    let scaleMapSecurity = d3.scaleLinear().domain([9, d3.max(viz.datasets.districts, (d) => parseFloat(d.Security))]).range(["#9ebcda", securityColor]);

    let scalePurchasePower = d3.scaleLinear().domain([0, d3.max(viz.datasets.districts, (d) => parseFloat(d.PurchasePower))]).range(["white", purchasePowerPurple]);

    viz.choropleth = new Choropleth(viz.activeFilter, scaleSellPrice, scaleRentPrice, scaleMapSecurity, scalePopulationDensity, scalePurchasePower);

    let svg = d3.select("#choropleth")
        .append("svg")
        .attr("width", width)
        .attr("height", height)
        .style("background-color", "white")
        .style("border-width", "1px")
        .style("border-radius", "5px")
        .style("padding", "10px")
        .style("box-shadow", "0 3px 10px rgb(0 0 0 / 0.2")
        .selectAll("path")
        .data(topojson.feature(data, data.objects.PRT_adm1).features)
        .join("path")
        .attr("class", "district")
        .attr("d", path)
        .attr("id", (d) => d.properties.NAME_1)
        .attr("fill", (d) => viz.choropleth.getActiveScale(d))
        .attr('stroke', 'white')
        .attr("stroke-width", 1)
        .on("mouseover", handleMouseOverChoropleth)
        .on("mouseleave", handleMouseLeaveChoropleth)
        .on("click", handleClickChoropleth)
        .on("mousemove", handleMouseMove)

    d3
        .select("#tooltip_choropleth")
        .style("position", "absolute")
        .style("visibility", "hidden")
        .style("background-color", "white")
        .style("border", "solid")
        .style("border-width", "1px")
        .style("border-radius", "5px")
        .style("padding", "10px")
}

function createLineChart(data, update) {
    width = 950;
    height = 198;
    margin = { top: 40, right:120, bottom: 20, left: 50 };

    sortData(data);
    viz.lineChart = new LineChart(sellPriceGreen, rentPriceBlue, viz.globalSelectedFilter);
    var line = 0
    var lines = []

    if(countDistricts() == 0) {
        line = d3.line().defined(data, function (d) {
            if(viz.globalSelectedFilter === "Sell") return d.AverageSellPrice > 0;
            else if(viz.globalSelectedFilter === "Rent") return d.AverageRentPrice > 0;
        }).x(function (d) {
            if(lineChartX === "Security") return x(parseFloat(d.Security));
            else if(lineChartX === "Density") return x(parseFloat(d.PopulationDensity));
            else if(lineChartX === "Power") return x(parseFloat(d.PurchasePower));
        }).y(function (d) {
            if(viz.globalSelectedFilter === "Sell") return y(parseFloat(d.AverageSellPrice));
            else if(viz.globalSelectedFilter === "Rent") return y(parseFloat(d.AverageRentPrice));
        });

        x = d3.scaleLinear().domain([d3.min(data, function(d) {
            if(lineChartX === "Security") return parseFloat(d.Security);
            else if(lineChartX === "Density") return parseFloat(d.PopulationDensity);
            else if(lineChartX === "Power") return parseFloat(d.PurchasePower);
        }), d3.max(data, function(d) {
            if(lineChartX === "Security") return parseFloat(d.Security);
            else if(lineChartX === "Density") return parseFloat(d.PopulationDensity);
            else if(lineChartX === "Power") return parseFloat(d.PurchasePower);
        })]).range([margin.left, width - margin.right]);

        y = d3.scaleLinear().domain([0, d3.max(data, function(d) {
            if(viz.globalSelectedFilter === "Sell") return parseFloat(d.AverageSellPrice);
            else if(viz.globalSelectedFilter === "Rent") return parseFloat(d.AverageRentPrice);
        })]).range([height - margin.bottom, margin.top]);
    }
    else {
        let minValueDistrictsX = 0
        let maxValueDistrictsX = 0
        let maxValueDistrictsY = 0
        let thisDistrictMinX = 0
        let thisDistrictMaxX = 0
        let thisDistrictMaxY = 0
        let numLines = 0
        lines = []
        for(let i = 0; i < districtLines.length; i++) {
            if(districtLines[i] != "") {
                let data_aux = dataSelectedDistricts[i];
                sortData(data_aux)
                lines[numLines] = d3.line().defined(dataSelectedDistricts[i], function (d) {
                    if(viz.globalSelectedFilter === "Sell") return d.AverageSellPrice > 0;
                    else if(viz.globalSelectedFilter === "Rent") return d.AverageRentPrice > 0;
                }).x(function (d) {
                    if(lineChartX === "Security") return x(parseFloat(d.Security));
                    else if(lineChartX === "Density") return x(parseFloat(d.PopulationDensity));
                    else if(lineChartX === "Power") return x(parseFloat(d.PurchasePower));
                }).y(function (d) {
                    if(viz.globalSelectedFilter === "Sell") return y(parseFloat(d.AverageSellPrice));
                    else if(viz.globalSelectedFilter === "Rent") return y(parseFloat(d.AverageRentPrice));
                });
                numLines += 1;

                thisDistrictMaxX = d3.max(dataSelectedDistricts[i], function(d) {
                    if(lineChartX === "Security") return  parseFloat(d.Security);
                    else if(lineChartX === "Density") return parseFloat(d.PopulationDensity);
                    else if(lineChartX === "Power") return parseFloat(d.PurchasePower);
                })
                if(thisDistrictMaxX > maxValueDistrictsX) maxValueDistrictsX = parseFloat(thisDistrictMaxX);

                thisDistrictMaxY = d3.max(dataSelectedDistricts[i], function(d) {
                    if(viz.globalSelectedFilter === "Sell") return parseFloat(d.AverageSellPrice);
                    else if(viz.globalSelectedFilter === "Rent") return parseFloat(d.AverageRentPrice);
                })
                if(thisDistrictMaxY > maxValueDistrictsY) maxValueDistrictsY = parseFloat(thisDistrictMaxY);
            }
        }
        minValueDistrictsX = maxValueDistrictsX;
        for(let i = 0; i < districtLines.length; i++) {
            if(districtLines[i] != "") {
                thisDistrictMinX = d3.min(dataSelectedDistricts[i], function(d) {
                    if(lineChartX === "Security") return parseFloat(d.Security);
                    else if(lineChartX === "Density") return parseFloat(d.PopulationDensity);
                    else if(lineChartX === "Power") return parseFloat(d.PurchasePower);
                })
                if(parseFloat(thisDistrictMinX) < minValueDistrictsX) { minValueDistrictsX = parseFloat(thisDistrictMinX); };
            }
        }

        x = d3.scaleLinear().domain([minValueDistrictsX, maxValueDistrictsX]).range([margin.left, width - margin.right]);
        y = d3.scaleLinear().domain([0, maxValueDistrictsY]).range([height - margin.bottom, margin.top]);
    }

    xAxis = (g) => g.attr("transform", `translate(0,${height - margin.bottom})`).call(d3.axisBottom(x).tickFormat((x) => x).tickSizeOuter(0));
    yAxis = (g) => g.attr("transform", `translate(${margin.left},0)`).call(d3.axisLeft(y).tickFormat((y) => y));

    if(!update) {
        d3.select("div#lineChart")
            .append("svg")
            .append("g")
            .attr("class", "line")
            .attr("fill", "steelblue")
            .append("path");
    }

    let svg = d3.select("div#lineChart")
            .select("svg")
            .attr("width", width)
            .attr("height", height)
            .style("background-color", "white")
            .style("padding", "10px");

    if(!update) {
        svg.append("g").attr("class", "lineXAxis");
        svg.append("g").attr("class", "lineYAxis");
    }

    svg.select("g.lineXAxis").call(xAxis);
    svg.select("g.lineYAxis").call(yAxis);

    if(countDistricts() == 0) {
        svg.selectAll("path#Portugal")
            .attr("visibility", "hidden");
        for(let i = 0; i < districtLines.length; i++)
            svg.selectAll("path#District"+i.toString()).attr("visibility", "hidden");
        svg.append("path")
            .attr("id", "Portugal")
            .datum(data)
            .attr("fill", "none")
            .attr("stroke", viz.lineChart.getColor())
            .attr("stroke-width", 1.5)
            .attr("stroke-linejoin", "round")
            .attr("stroke-linecap", "round")
            .attr("d", line);
    }
    else {
        svg.selectAll("path#Portugal")
            .attr("visibility", "hidden");
        for(let i = 0; i < districtLines.length; i++)
            svg.selectAll("path#District"+i.toString()).attr("visibility", "hidden");
        for(let i = 0; i < districtLines.length; i++) {
            let numLines = 0;
            if(districtLines[i] !== "") {
                svg.append("path")
                    .attr("id", "District"+i.toString())
                    .datum(dataSelectedDistricts[i])
                    .attr("fill", "none")
                    .attr("stroke", linesColors[i])
                    .attr("stroke-width", 1.5)
                    .attr("stroke-linejoin", "round")
                    .attr("stroke-linecap", "round")
                    .attr("d", lines[numLines]);
                numLines += 1;
            }
        }
    }

    svg.select("g.line")
        .selectAll("line")
        .data(data, function(d) { return d.AverageSellPrice; })
        .join(
            (enter) => {
                lineChartDropdownMenus()
                return enter.append("line").attr("x", function (d) {
                    if(lineChartX === "Security") return x(parseFloat(d.Security));
                    else if(lineChartX === "Density") return x(parseFloat(d.PopulationDensity));
                    else if(lineChartX === "Power") return x(parseFloat(d.PurchasePower));
                }).attr("y", function (d) {
                    if(viz.globalSelectedFilter === "Sell") return y(parseFloat(d.AverageSellPrice));
                    else if(viz.globalSelectedFilter === "Rent") return y(parseFloat(d.AverageRentPrice));
                });
            },
            (update) => {
                lineChartDropdownMenus();
                if(dataSelectedDistricts.length == 0) {
                    sortData(data);
                    update.attr("x", function (d) {
                        if(lineChartX === "Security") return x(parseFloat(d.Security));
                        else if(lineChartX === "Density") return x(parseFloat(d.PopulationDensity));
                        else if(lineChartX === "Power") return x(parseFloat(d.PurchasePower));
                    }).attr("y", function (d) {
                        if(viz.globalSelectedFilter === "Sell") return y(parseFloat(d.AverageSellPrice));
                        else if(viz.globalSelectedFilter === "Rent") return y(parseFloat(d.AverageRentPrice));
                    });
                }
                else {
                    for(let i = 0; i < dataSelectedDistricts.length; i++) {
                        sortData(dataSelectedDistricts[i]);
                        update.attr("x", function (d) {
                            if(lineChartX === "Security") return x(d.Security);
                            else if(lineChartX === "Density") return x(d.PopulationDensity);
                            else if(lineChartX === "Power") return x(d.PurchasePower);
                        }).attr("y", function (d) {
                            if(viz.globalSelectedFilter === "Sell") return y(d.AverageSellPrice);
                            else if(viz.globalSelectedFilter === "Rent") return y(d.AverageRentPrice);
                        });
                    }
                }

            },
            (exit) => {
                exit.remove();
            }
        );
}

function createHistogram(data, update) {
	width = 400;
    height = 200;
	margin = { top: 30, right:20, bottom: 40, left: 20 };
	xRange = [margin.left, width - margin.right];
	yRange = [margin.top, height - margin.bottom];
	sellDomain = [0, 800000]
	rentDomain = [0, 4000]
	color = (viz.globalSelectedFilter == 'Rent') ? rentPriceBlue : sellPriceGreen;

    if (update && viz.selectedDistricts.length > 0) {
		data = data.filter((d) => {
				return viz.selectedDistricts.includes(d.District.substr(1));

			}
		)
	}

    data = data.filter((d) => {
        if (d.AdsType === viz.globalSelectedFilter) {
            return d.Price;
        }
    })

    if(!update) {

        sellPriceMax = d3.max(data, function(d) { return parseFloat(d.Price);} )
        svg = d3
            .select("div#histogram")
            .append("svg")
            .attr("width", width)
            .attr("height", height)
            .append("g")
            .attr("class", "bar")
    }
    sellDomain = [0, sellPriceMax]
    domain = (viz.globalSelectedFilter == 'Rent') ? rentDomain : sellDomain;

    const xScale = d3.scaleLinear()
        .domain(domain)
        .range(xRange);
    var histogram = d3.histogram()
    .value(function(d) {
        return parseFloat(d.Price);
    })
    .domain(domain)
    .thresholds(xScale.ticks(50));

    if (!update) {
        brushSelected = xScale.domain()
    }

    var bins = histogram(data);
    const yScale = d3.scaleLinear()
        .domain([d3.max(bins, function(d) {
            return d.length; }), 0])
        .range(yRange);

    function xAxis(g) {
        g.attr("transform", `translate(0, ${height - margin.bottom})`).call(d3.axisBottom(xScale).ticks(10));
    }

    svg = d3
        .select("div#histogram")
        .select("svg")
        .attr("width", width)
        .attr("height", height);

    svg.select("g.bar")
		.selectAll("rect")
        .attr("fill", color)
        .data(bins)
        .join(
            (enter) => {
                return enter.append("rect")
                .attr("fill", color)
                .attr("x", 1)
                .attr("transform", function(d) { return "translate(" + xScale(d.x0) + "," + yScale(d.length) + ")"; })
                .attr("width", (d) => {
                    barSize = xScale(d.x1) - xScale(d.x0) - 1
                    if (barSize > 0) return barSize
                })
                .attr("height", (d) => yScale(0) - yScale(d.length))
            },
            (update) => {
                update
                .attr("x", 1)
                .transition()
                .duration(1000)
                .attr("transform", function(d) { return "translate(" + xScale(d.x0) + "," + yScale(d.length) + ")"; })
                .attr("width", (d) => {
                    barSize = xScale(d.x1) - xScale(d.x0) - 1
                    if (barSize > 0) return barSize
                })
                .attr("height", (d) => yScale(0) - yScale(d.length))
            },
            (exit) => {
                exit.remove(); }
        )
    if(!update) {
        svg
        .append("g").attr("class", "xAxis").call(xAxis);
    }
    svg.select("g.xAxis").call(xAxis);

	//BRUSHING

    svg.append("g")
	.attr("class", "brush")
	.call(d3.brushX()
    .extent([[margin.left, margin.top], [width - margin.left, height - margin.bottom]])
    .on("start brush end", brushed))

	if (update) {
		svg.select("g.brush").remove();
	}

    function brushed() {
        var extent = this.__brush.selection
        brushSelected = ( extent == null) ? domain : [xScale.invert(extent[0][0]), xScale.invert(extent[1][0])]
        svg.select("g.bar")
		.selectAll("rect").classed("hidden", function(d) {
            return( extent == null ? false : d.x0 >= brushSelected[0] && d.x1 <= brushSelected[1] ? false : true)
        });
        svg.select("g.bar").selectAll("rect").attr('fill',color)
        svg.select("g.bar").selectAll("rect.hidden").attr('fill','lightgrey')
        onBrushed(brushSelected);
    }
}

function createBoxPlots(update) {
    if (!update) {
        bathsMax = d3.max(bathsSortedData, function(d) { return parseFloat(d.Bathrooms);} )
        roomsMax = d3.max(roomsSortedData, function(d) { return parseFloat(d.Rooms);} )
        areaMax = d3.max(areaSortedData, function(d) { return parseFloat(d.PropertyArea);} )
    }
    createBoxPlot(bathsSortedData, update, "#bathsPlot")
    createBoxPlot(roomsSortedData, update, "#roomsPlot")
    createBoxPlot(areaSortedData, update, "#areaPlot")
}

function onBrushed() {
    createBoxPlots(true)
}

function createBoxPlot(data, update, div) {
    if (update && viz.selectedDistricts.length > 0) {
		data = data.filter((d) => {
				return viz.selectedDistricts.includes(d.District.substr(1));

			}
		)
	}
    data = data.filter((d) => {
        if (d.AdsType === viz.globalSelectedFilter && d.Price >= brushSelected[0] && d.Price <= brushSelected[1]) {
            if (div == "#bathsPlot") return d.Bathrooms;
            if (div == "#roomsPlot") return d.Rooms;
            if (div == "#areaPlot") return d.PropertyArea;
        }
    }).map(function(d) {
        if (div == "#bathsPlot") return d.Bathrooms;
        if (div == "#roomsPlot") return d.Rooms;
        if (div == "#areaPlot") return d.PropertyArea;
    })
    if (data.length != 0) drawBoxPlot(data, update, div)
    else {
        cleanBoxPlot(div)
    }
}

function drawBoxPlot(data, update, div) {

    var margin = {top: 10, right: 20, bottom: 10, left: 30},
    width = 560 / 3
    height = 180 - margin.top - margin.bottom;
    color = (viz.globalSelectedFilter == 'Rent') ? rentPriceBlue : sellPriceGreen;

    if(!update) {
        var svg = d3.select(div)
        .append("svg")
        .attr("width", width)
        .attr("height", height + margin.top + margin.bottom)
        .append("g")
        .attr("class", "box")
        svg.append("g").attr("class", "verticalLine")
        svg.append("g").attr("class", "rect")
        svg.append("g").attr("class", "horizontalLine")
    }

    var svg = d3.select(div)
    .select("svg")
    .select("g")
    .attr("transform",
                "translate(" + margin.left + "," + margin.top + ")")
    var q1 = d3.quantile(data, .25)
    var median = d3.quantile(data, .5)
    var q3 = d3.quantile(data, .75)
    min = d3.min(data, function(d) { return parseFloat(d);} )
    max = d3.max(data, function(d) { return parseFloat(d);} )
    var y = d3.scaleLinear()
    .domain([0, (() => {
        switch (div) {
            case "#bathsPlot": return bathsMax;
            case "#roomsPlot": return roomsMax;
            case "#areaPlot": return areaMax;
        }
    })()])
    .range([height, 0]);
    var ticks = (div == "#areaPlot") ? 15 : max

    svg
    .call(d3.axisLeft(y).ticks(ticks))
    var center = 83
    var width = 75

    if (update) {
        cleanBoxPlot(div)
    }

    svg
    .select("g.verticalLine")
    .append("line")
        .attr("x1", center)
        .attr("x2", center)
        .attr("y1", y(min) )
        .attr("y2", y(max) )
        .attr("stroke", "black")

// Show the box
    svg
    .select("g.rect")
    .append("rect")
        .attr("x", center - width/2)
        .attr("y", y(q3) )
        .attr("height", (y(q1)-y(q3)) )
        .attr("width", width )
        .transition()
        .duration(1000)
        //.attr("stroke", "black")
        .style("fill", color)

    svg
    .select("g.horizontalLine")
        .selectAll("line")
        .data([min, max, median])
        .enter()
        .append("line")
        .attr("x1", center-width/2)
        .attr("x2", center+width/2)
        .attr("y1", (d) => y(d) )
        .attr("y2", (d) => y(d) )
        .transition()
        .duration(1000)
        .attr("stroke", "black")


}

function cleanBoxPlot(div) {
    var svg = d3.select(div)
    svg
    .select("g.rect")
    .selectAll("rect")
    .remove()
   svg
    .select("g.verticalLine")
    .selectAll("line")
    .remove()
   svg
    .select("g.horizontalLine")
    .selectAll("line")
    .remove()
}

function createRadarChart(data, update) {
    width = 610;
    height = 255;
    margin = { top: 40, right:120, bottom: 20, left: 50 };

    d3.selectAll("#colorsLegend")
        .attr("visibility", "hidden");

    viz.radarChart = new RadarChart();

    const labels = ['Security', 'Purchase Power', 'Sell Price', 'Rent Price', 'Population Density'];
    const radius = 100;
    const center = {x: 400, y: 140};

    let averageSecurity = 0;
    let averagePurchasePower = 0;
    let averageSellPrice = 0;
    let averageRentPrice = 0;
    let averagePopulationDensity = 0;

    data.forEach((d) => {
        averageSecurity += (parseFloat(d.Security)) / data.length;
        averagePurchasePower += parseFloat(d.PurchasePower) / data.length;
        averageSellPrice += parseFloat(d.AverageSellPrice) / data.length;
        averageRentPrice += parseFloat(d.AverageRentPrice) / data.length;
        averagePopulationDensity += parseFloat(d.PopulationDensity) / data.length;
    });

    let maxSecurity = d3.max(data, (d) => parseFloat(d.Security))
    let maxPurchasePower = d3.max(data, (d) => parseFloat(d.PurchasePower))
    let maxSellPrice = d3.max(data, (d) => parseFloat(d.AverageSellPrice))
    let maxRentPrice = d3.max(data, (d) => parseFloat(d.AverageRentPrice))
    let maxPopulationDensity = d3.max(data, (d) => parseFloat(d.PopulationDensity))

    let minSecurity = d3.min(data, (d) => parseFloat(d.Security))
    let minPurchasePower = d3.min(data, (d) => parseFloat(d.PurchasePower))
    let minSellPrice = d3.min(data, (d) => parseFloat(d.AverageSellPrice))
    let minRentPrice = d3.min(data, (d) => parseFloat(d.AverageRentPrice))
    let minPopulationDensity = d3.min(data, (d) => parseFloat(d.PopulationDensity))

    let minValues = [minSecurity, minPurchasePower, minSellPrice, minRentPrice, minPopulationDensity];
    let maxValues = [maxSecurity, maxPurchasePower, maxSellPrice, maxRentPrice, maxPopulationDensity];
    let dataRadar = [averageSecurity, averagePurchasePower, averageSellPrice, averageRentPrice, averagePopulationDensity];

    if(!update) {
        d3.select("div#radarChart")
            .append("svg")
            .append("g")
            .attr("class", "line")
            .attr("fill", "steelblue")
            .append("path");
    }

    let svg = d3.select("div#radarChart")
                .select("svg")
                .attr("width", width)
                .attr("height", height);

    for(let i = 0; i < dataRadar.length; i++) {
        let ticks = [minValues[i]*0.8+maxValues[i]*0.2, minValues[i]*0.6+maxValues[i]*0.4, minValues[i]*0.4+maxValues[i]*0.6,
            minValues[i]*0.2+maxValues[i]*0.8, maxValues[i]];
        ticks.forEach(function (t) {
            svg.append("circle")
                .attr("cx", center.x)
                .attr("cy", center.y)
                .attr("fill", "none")
                .attr("stroke", "gray")
                .attr("r", getRadialScale(minValues[i], maxValues[i], radius, t))
        }
        );
    }

    let angle = 0;
    let lineX = 0;
    let lineY = 0;
    let labelX = 0;
    let labelY = 0;
    let alignment = 0;

    if(!update) {
        for (let i = 0; i < labels.length; i++) {
            if(i === 0) alignment = 200;
            if(i === 1) alignment = maxValues[i]+10;
            else if(i === 2) alignment = maxValues[i]+90000;
            else if(i === 3) alignment = maxValues[i]+2300;
            else alignment = maxValues[i]+6000;
            angle = (Math.PI / 2) - (2 * Math.PI * i / labels.length);
            lineX = center.x + Math.cos(angle) * getRadialScale(minValues[i], maxValues[i], radius, maxValues[i]);
            lineY = center.y - Math.sin(angle) * getRadialScale(minValues[i], maxValues[i], radius, maxValues[i]);
            labelX = center.x + Math.cos(angle) * getRadialScale(minValues[i], maxValues[i], radius, alignment);
            if (labels[i] === "Security")
                labelY = center.y - Math.sin(angle) * getRadialScale(minValues[i], maxValues[i], radius, maxValues[i]) - 10;
            else
                labelY = center.y - Math.sin(angle) * getRadialScale(minValues[i], maxValues[i], radius, maxValues[i]);
            svg.append("line")
                .attr("id", "attribute"+i.toString())
                .attr("x1", center.x)
                .attr("y1", center.y)
                .attr("x2", lineX)
                .attr("y2", lineY)
                .attr("stroke","black");
            svg.append("text")
                .attr("id", labels[i])
                .attr("x", labelX)
                .attr("y", labelY)
                .text(labels[i]);
        }
    }

    //For plotting the data

    let line = d3.line()
        .x(d => d.x)
        .y(d => d.y);

    let coordinate = {"x": 0, "y": 0};
    let coordinates = [];

    svg.select("path#radar")
        .selectAll("path")
        .attr("visibility", "hidden");

    for (let i = 0; i < districtLines.length; i ++) {
        if(districtLines[i] != "") {
            svg.select(districtLines[i])
                .selectAll("path")
                .attr("visibility", "hidden");
        }
    }

    if(countDistricts() === 0) {
        svg.selectAll("path")
            .attr("visibility", "hidden");
        for (let i = 0; i < labels.length; i++){
            angle = (Math.PI / 2) - (2 * Math.PI * i / labels.length);
            coordinate = {"x": center.x + Math.cos(angle) * getRadialScale(minValues[i], maxValues[i], radius, dataRadar[i]),
                "y": center.y - Math.sin(angle) * getRadialScale(minValues[i], maxValues[i], radius, dataRadar[i])};
            coordinates.push(coordinate);
        }
        svg.append("path")
            .attr("id", "radar")
            .datum(coordinates)
            .attr("d",line)
            .attr("stroke-width", 3)
            .attr("stroke", viz.globalSelectedFilter === "Sell"? sellPriceGreen : rentPriceBlue)
            .attr("fill", viz.globalSelectedFilter === "Sell"? sellPriceGreen : rentPriceBlue)
            .attr("stroke-opacity", 1)
            .attr("opacity", 0.5);
    }
    else {
        let dataDistrictRadar = [];
        svg.selectAll("path")
            .attr("visibility", "hidden");
        for (let i = 0; i < districtLines.length; i ++) {
            if(districtLines[i] != "") {
                coordinates = [];
                line = d3.line()
                    .x(d => d.x)
                    .y(d => d.y);
                data.filter(function (d) {
                    if(d.District.substr(1) === districtLines[i]){
                        dataDistrictRadar = [d.Security, d.PurchasePower, d.AverageSellPrice, d.AverageRentPrice, d.PopulationDensity];
                    }
                })
                for (let i = 0; i < dataDistrictRadar.length; i++){
                    angle = (Math.PI / 2) - (2 * Math.PI * i / labels.length);
                    coordinate = {"x": center.x + Math.cos(angle) * getRadialScale(minValues[i], maxValues[i], radius, dataDistrictRadar[i]),
                        "y": center.y - Math.sin(angle) * getRadialScale(minValues[i], maxValues[i], radius, dataDistrictRadar[i])};
                    coordinates.push(coordinate);
                }
                svg.append("path")
                    .attr("id", districtLines[i])
                    .datum(coordinates)
                    .attr("d",line)
                    .attr("stroke-width", 3)
                    .attr("stroke", linesColors[i])
                    .attr("fill", linesColors[i])
                    .attr("stroke-opacity", 1)
                    .attr("opacity", 0.5);
            }
        }
    }

    //Creation of the selected districts legend
    if (!update) {
        let legendSvg = d3.select("div#districtsLegend")
                        .append("svg")
                        .attr("height", height);
    }
    let legendSvg = d3.select("div#districtsLegend")
                    .select("svg")

    for(let i = 0; i < districtLines.length; i++) {
        legendSvg.selectAll("circle#legendCircle"+i.toString()).attr("visibility", "hidden");
        legendSvg.selectAll("text#legendText"+i.toString()).attr("visibility", "hidden");
    }

    if(countDistricts() == 0) {
        legendSvg.append("circle")
            .attr("id", "legendCircle0")
            .attr("cx",115)
            .attr("cy",60)
            .attr("r", 6)
            .style("fill", viz.globalSelectedFilter === "Sell"? sellPriceGreen : rentPriceBlue);
        legendSvg.append("text")
            .attr("id", "legendText0")
            .attr("x", 125)
            .attr("y", 60)
            .text("Portugal")
            .style("font-size", "12px")
            .style("font-family", "helvetica, sans-serif")
            .attr("alignment-baseline","middle");
    }
    let numDistricts = 0;
    for(let i = 0; i < districtLines.length; i++) {
        if(districtLines[i] != "") {
            legendSvg.append("circle")
                .attr("id", "legendCircle"+i.toString())
                .attr("cx",115)
                .attr("cy",60+40*numDistricts)
                .attr("r", 6)
                .style("fill", linesColors[i]);
            legendSvg.append("text")
                .attr("id", "legendText"+i.toString())
                .attr("x", 125)
                .attr("y", 60+40*numDistricts)
                .text(districtLines[i])
                .style("font-size", "12px")
                .style("font-family", "helvetica, sans-serif")
                .attr("alignment-baseline","middle");
            numDistricts++;
        }
    }
}

function handleMouseOverChoropleth(event, d) {
    d3.select("#choropleth").selectAll("path").filter((p) => p.properties.NAME_1 === d.properties.NAME_1).attr("fill", "red")
    let tooltip = d3.select("div#tooltip_choropleth")
    tooltip.select("#location-tooltip").text("Location: " + d.properties.NAME_1)
    tooltip.select("#sell-price").text("Sell Price: " +  parseFloat(d.properties.AverageSellPrice).toFixed(2))
    tooltip.select("#rent-price").text("Rent Price: " +  parseFloat(d.properties.AverageRentPrice).toFixed(2))
    tooltip.select("#security_tooltip").text("Security: " +  parseFloat(d.properties.Security).toFixed(2))
    tooltip.select("#pop-density").text("Population Density: " +  parseFloat(d.properties.PopulationDensity).toFixed(2))
    tooltip.select("#purchase-power").text("Purchase Power: " +  parseFloat(d.properties.PurchasePower).toFixed(2))
    tooltip.style("visibility", "visible");
    tooltip.style("top", event.pageY + "px")
    tooltip.style("left", event.pageX + "px")
}

function handleMouseLeaveChoropleth(event, d) {
    d3.select("#choropleth").selectAll("path").filter((p) => p.properties.NAME_1 === d.properties.NAME_1).attr("fill", (d) => viz.choropleth.getActiveScale(d) )

    let tooltip = d3.select("div#tooltip_choropleth").style("visibility", "hidden");
}

function handleMouseMove(event, d) {
    d3.select("div#tooltip_choropleth").style("top", event.pageY + 10 + "px").style("left", event.pageX + 10 + "px")
}

function handleClickChoropleth(event, d) {
    if (viz.selectedDistricts.includes(d.properties.NAME_1)) { //In case its getting unselected
        viz.selectedDistricts = viz.selectedDistricts.filter((p) => p !== d.properties.NAME_1);
        resetColourChoropleth(d, 2)
		resetHistogram()
        brushSelected = (viz.globalSelectedFilter === 'Rent') ? rentDomain : sellDomain
        createBoxPlots(true)
        filterLineChart(locationsParishesInfo, d.properties.NAME_1, false);
        createRadarChart(locationsDistricts, true);
    } else {
        if (viz.selectedDistricts.length < 5) {
            viz.selectedDistricts.push(d.properties.NAME_1);
            resetColourChoropleth(d, 1)
            d3.select("#choropleth").selectAll("path").filter((p) => p.properties.NAME_1 === d.properties.NAME_1).attr("stroke", linesColors[viz.selectedDistricts.length-1]).attr("stroke-width", 1.5).raise();
            //TODO selected a new one, change maps to it
            resetHistogram();
            brushSelected = (viz.globalSelectedFilter === 'Rent') ? rentDomain : sellDomain
            filterLineChart(locationsParishesInfo, d.properties.NAME_1, true);
            createRadarChart(locationsDistricts, true);
        } else {
            alert("Too many districts selected, unselect some")
        }

    }
}

function resetColourChoropleth(d, type) {
    switch (type) {
        case 1: //In case it's just a local reset and not deselection
            d3.select("#choropleth").selectAll("path").filter((p) => p.properties.NAME_1 === d.properties.NAME_1).attr("fill", (d) => viz.choropleth.getActiveScale(d))
            break;
        case 2: // in case we're not unselecting
            d3.select("#choropleth").selectAll("path").filter((p) => p.properties.NAME_1 === d.properties.NAME_1).attr("fill", (d) => viz.choropleth.getActiveScale(d)).attr("stroke", "white").attr("stroke-width", 1).lower()
            break;
        case 3: // resetting the map
            d3.select("#choropleth").selectAll("path").attr("fill", (d) => viz.choropleth.getActiveScale(d)).attr("stroke", "white").lower()
            break;
    }
}

function resetHistogram(){
 d3.csv(adsDataset)
        .then((data) => {
        	createHistogram(data, true);
		})
}

function addDistrict(district) {
    for(let i = 0; i < districtLines.length; i++)
        if(districtLines[i] == "") {
            districtLines[i] = district;
            break;
        }
}

function removeDistrict(district) {
    for(let i = 0; i < districtLines.length; i++)
        if(districtLines[i] == district) {
            districtLines[i] = "";
            break;
        }
}

function countDistricts() {
    let count = 0;
    for(let i = 0; i < districtLines.length; i++)
        if(districtLines[i] != "")
            count += 1;
    return count;
}

function filterLineChart(dataset, selectedDistrict, add){
    filteredDistricts = []
    d3.select("div#lineChart").selectAll("path").remove();
    if(add) addDistrict(selectedDistrict);
    else removeDistrict(selectedDistrict);
    for(let i = 0; i < districtLines.length; i++){
        if(districtLines[i] != "") {
            dataSelectedDistricts[i] = dataset.filter(function (d) {
                if(d.District.substr(1) === districtLines[i]){
                    return d.District;
                }
            })
        }
        else dataSelectedDistricts[i] = [];
    }
    createLineChart(dataset, true);
}

function filterGlobal(value) {
    viz.globalSelectedFilter = value;
	viz.selectedDistricts = [];
    resetHistogram();
    brushSelected = (value === 'Rent') ? rentDomain : sellDomain
    createBoxPlots(true);
    districtLines = ["", "", "", "", ""];
    dataSelectedDistricts = [[], [], [], [], []];

    d3.csv("data/parishes.csv")
        .then((newData) => {
            dataChangeLineChart(value);
            createLineChart(newData, true);
        })
    d3.csv("data/locations_districts.csv")
        .then((newData) => {
            createRadarChart(newData, true);
        })
    viz.activeFilter = value;
    document.getElementById("PurchasePower").checked=false
    document.getElementById("Security").checked=false
    document.getElementById("Population").checked=false
    disableAllScales()
    value === 'Rent' ? d3.select('#colorscale_choropleth_rent').select("svg").attr("visibility", "visible") : d3.select('#colorscale_choropleth_sale').select("svg").attr("visibility", "visible")
        viz.choropleth.changeScale(value, true )
}

function filterChoropleth(value) {
    disableAllScales()
    switch (value) {
        case "Security":
            document.getElementById("Population").checked=false
            document.getElementById("PurchasePower").checked=false
            break;
        case "Population":
            document.getElementById("Security").checked=false
            document.getElementById("PurchasePower").checked=false
            break;
        case "PurchasePower":
            document.getElementById("Security").checked=false
            document.getElementById("Population").checked=false
            break;

    }
    viz.choropleth.changeScale(value, false)
}

function dataChangeLineChart(value) {
    d3.csv("data/parishes.csv").then((data) => {
        let newData = data;
        switch (value) {
            case "density":
                lineChartX = "Density";
                break;
            case "security":
                lineChartX = "Security";
                break;
            case "power":
                lineChartX = "Power";
                break;
            default:
                break;
            }
            createLineChart(newData, true);
        }).catch((error) => { console.log(error); });
}

function lineChartDropdownMenus() {
    Xmenu = d3.select("div#Xmenu");
    Xlegend = d3.select("div#xAxisLegend");
    Ylegend = d3.select("div#yAxisLegend");
    if(lineChartX === "Security") { Xmenu.text("Security"); Xlegend.text("Security"); }
    else if(lineChartX === "Density") { Xmenu.text("Population Density"); Xlegend.text("Population Density"); }
    else if(lineChartX === "Power") { Xmenu.text("Purchase Power"); Xlegend.text("Purchase Power"); }
    if(viz.globalSelectedFilter === "Sell") { Ylegend.text("Sell Price"); }
    else if(viz.globalSelectedFilter === "Rent") { Ylegend.text("Rent Price"); }
}

function getRadialScale(minValue, maxValue, radius, value) {
    radialScale = d3.scaleLinear()
            .domain([minValue, maxValue])
            .range([0, radius]);
    return radialScale(value);
}

function sortData(data) {
    if(lineChartX === "Security") data.sort((a, b) => d3.ascending(parseFloat(a.Security), parseFloat(b.Security)));
    else if(lineChartX === "Density") data.sort((a, b) => d3.ascending(parseFloat(a.PopulationDensity), parseFloat(b.PopulationDensity)));
    else if(lineChartX === "Power") data.sort((a, b) => d3.ascending(parseFloat(a.PurchasePower), parseFloat(b.PurchasePower)));
}

function numberFormatter(number) {
    switch (number.toString().split(".")[0].length) {
        case 2:
            return Math.ceil(number/10) / 10
        case 3:
            return parseInt(Math.ceil(number/100)*10)
        case 4:
            return parseInt(Math.ceil(number/1000)*1000)
        case 5:
            return parseInt(Math.ceil(number/10000)*100)
        case 6:
            return parseInt(Math.ceil(number/100000)*10)
        case 7:
            return parseInt(Math.ceil(number / 100000))
        default:
            return Math.ceil(number/10)

    }
}

function disableAllScales() {
    scales = ['colorscale_choropleth_security', 'colorscale_choropleth_population', 'colorscale_choropleth_purchase_power', 'colorscale_choropleth_sale', 'colorscale_choropleth_rent']
    for (let i = 0; i < scales.length; i++) {
        var svgLegend = d3.select('#' + scales[i]).select('svg').attr("visibility", "hidden");
    }
}
