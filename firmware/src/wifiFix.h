#pragma once

class WiFiClientFixed : public WiFiClient {
public:
	void flush() override;
};