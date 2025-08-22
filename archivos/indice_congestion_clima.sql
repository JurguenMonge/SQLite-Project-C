CREATE INDEX IF NOT EXISTS  idx_congestion_weather_cond
    ON Congestion(Weather_Conditions);
CREATE INDEX IF NOT EXISTS  idx_congestion_weather_event
    ON Congestion(Weather_Event);
CREATE INDEX IF NOT EXISTS  idx_congestion_weather_cover
    ON Congestion(Weather_Conditions, Weather_Event, Severity, StartTime, EndTime);