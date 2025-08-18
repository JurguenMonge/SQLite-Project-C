CREATE TABLE "Congestion" (
	"ID"	TEXT,
	"Severity"	TEXT,
	"Start_Lat"	TEXT,
	"Start_Lng"	TEXT,
	"StartTime"	TEXT,
	"EndTime"	TEXT,
	"Distance(mi)"	TEXT,
	"DelayFromTypicalTraffic(mins)"	TEXT,
	"DelayFromFreeFlowSpeed(mins)"	TEXT,
	"Congestion_Speed"	TEXT,
	"Description"	TEXT,
	"Street"	TEXT,
	"City"	TEXT,
	"County"	TEXT,
	"State"	TEXT,
	"Country"	TEXT,
	"ZipCode"	TEXT,
	"LocalTimeZone"	TEXT,
	"WeatherStation_AirportCode"	TEXT,
	"WeatherTimeStamp"	TEXT,
	"Temperature(F)"	TEXT,
	"WindChill(F)"	TEXT,
	"Humidity(%)"	TEXT,
	"Pressure(in)"	TEXT,
	"Visibility(mi)"	TEXT,
	"WindDir"	TEXT,
	"WindSpeed(mph)"	TEXT,
	"Precipitation(in)"	TEXT,
	"Weather_Event"	TEXT,
	"Weather_Conditions"	TEXT
);

.mode csv
.separator ,
.import --skip 1 "C:/Users/Jurguen Monge/CLionProjects/SqlProject/archivos/Congestion.csv" Congestion
.exit
