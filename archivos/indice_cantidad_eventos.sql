CREATE INDEX IF NOT EXISTS idx_congestion_basic
ON Congestion (Severity, State, StartTime, Weather_Conditions);