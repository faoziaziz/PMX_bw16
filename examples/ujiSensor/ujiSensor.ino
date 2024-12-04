#include <PMX_bw16.h>
#include <PMX_aht10.h>

AHT10 pmxAHT(AHT10_ADDRESS_0X38,AHT10_SENSOR);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  while(pmxAHT.begin()!=true){
    Serial.println("Gak bisa kedetek aht10 nya ");
    delay(5000);
  }

  Serial.println("Aht10 kedetek");
}

void loop() {
  // put your main code here, to run repeatedly:
  

  Serial.println("baca 12 bytes");
  Serial.print("temperature : "); Serial.print(pmxAHT.readTemperature());
  Serial.println();
  Serial.print("humidity : "); Serial.print(pmxAHT.readHumidity());
  Serial.println();
  pmx_msg();
  delay(1000);

}
