### Testing of TCRT5000 IR Sensor


# this sensor is found here -> https://www.amazon.com/dp/B00LZV1V10?ref=ppx_yo2ov_dt_b_fed_asin_title
# it has the datasheet here -> https://www.vishay.com/docs/83760/tcrt5000.pdf
# The device uses an op amp the LM393n -> https://www.ti.com/lit/ds/symlink/lm393.pdf?ts=1762757993313&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FLM393%253Futm_source%253Dgoogle%2526utm_medium%253Dcpc%2526utm_campaign%253Dti-null-null-xref-cpc-pf-google-ww_en_cons%2526utm_content%253Dxref%2526ds_k%253DLM393S%2526dcm%253Dyes%2526gclsrc%253Daw.ds%2526gad_source%253D1%2526gad_campaignid%253D23167718368%2526gbraid%253D0AAAAAC068F1WLrT66ZpX_2WnHuW-aVtPH%2526gclid%253DEAIaIQobChMIjsDSioHnkAMVHTbUAR3AUw-YEAAYAiAAEgLSffD_BwE

### How it works
# the TCRT5000 sensor works by sending an inf. light and if the light is detected the Sensor
# gives a var. voltage this is compared to a pot. value that can be manually ajusted and seen
# either on analog or digital pin.
# the orange does not have an ADC to read on the board
# however, there are ways to read the

### What is included in this testing module
# the code has a header file witch includes main.c which is a testing function
# the second line is a folder that can be copied and used in place within any peice of code
# the folder is called interface code.
