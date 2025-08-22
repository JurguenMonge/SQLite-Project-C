DROP VIEW IF EXISTS view_congestion_clima;
CREATE VIEW IF NOT EXISTS view_congestion_clima AS
WITH base AS (
  SELECT
    CASE
      WHEN Weather_Conditions LIKE '%Snow%' OR Weather_Event LIKE '%Snow%' THEN 'Snow'
      WHEN Weather_Conditions LIKE '%Thunder%' OR Weather_Conditions LIKE '%Storm%' THEN 'Storm'
      WHEN Weather_Conditions LIKE '%Rain%' OR Weather_Conditions LIKE '%Drizzle%' THEN 'Rain'
      WHEN Weather_Conditions LIKE '%Fog%'  OR Weather_Conditions LIKE '%Mist%' OR Weather_Conditions LIKE '%Haze%' THEN 'Fog/Mist'
      WHEN Weather_Conditions LIKE '%Clear%' THEN 'Clear'
      WHEN Weather_Conditions LIKE '%Cloud%' THEN 'Clouds'
      ELSE 'Other'
    END AS weather_group,
    Severity,
    (julianday(EndTime)-julianday(StartTime))*1440.0 AS duration_min
  FROM Congestion
)
SELECT
  weather_group,
  COUNT(*) AS events,
  ROUND(AVG(Severity),2) AS avg_severity,
  ROUND(AVG(duration_min),2) AS avg_duration_min
FROM base
GROUP BY weather_group;