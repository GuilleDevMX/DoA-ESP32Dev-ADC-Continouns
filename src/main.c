#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_adc/adc_continuous.h"
#include "esp_adc/adc_oneshot.h"
#include "soc/soc_caps.h"

#define TAG "ADC_CONT"
#define NUM_MIC_CHANNELS 5
#define SAMPLE_RATE 80000
#define DMA_BUFFER_SIZE 1000

static adc_continuous_handle_t adc_handle = NULL;
static uint8_t adc_read_buffer[DMA_BUFFER_SIZE];
static int16_t mic_data[NUM_MIC_CHANNELS][DMA_BUFFER_SIZE / NUM_MIC_CHANNELS];

static void adc_continuous_init(void);
static void process_adc_samples(uint8_t *buffer, size_t bytes_read);

static void adc_continuous_init(void)
{
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 2048,
        .conv_frame_size = DMA_BUFFER_SIZE,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &adc_handle));

    adc_digi_pattern_config_t adc_patterns[NUM_MIC_CHANNELS] = {
        {.atten = ADC_ATTEN_DB_12, .channel = 4, .unit = ADC_UNIT_1, .bit_width = 12}, // GPIO32
        {.atten = ADC_ATTEN_DB_12, .channel = 5, .unit = ADC_UNIT_1, .bit_width = 12}, // GPIO33
        {.atten = ADC_ATTEN_DB_12, .channel = 6, .unit = ADC_UNIT_1, .bit_width = 12}, // GPIO34
        {.atten = ADC_ATTEN_DB_12, .channel = 7, .unit = ADC_UNIT_1, .bit_width = 12}, // GPIO35
        {.atten = ADC_ATTEN_DB_12, .channel = 0, .unit = ADC_UNIT_1, .bit_width = 12}, // GPIO36
    };
    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = SAMPLE_RATE,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
        .pattern_num = NUM_MIC_CHANNELS,
        .adc_pattern = adc_patterns,
    };
    ESP_ERROR_CHECK(adc_continuous_config(adc_handle, &dig_cfg));
    ESP_LOGI(TAG, "ADC continuous mode initialized at %d Hz sample rate.", SAMPLE_RATE);
}

static void process_adc_samples(uint8_t *buffer, size_t bytes_read)
{
    int num_results = bytes_read / sizeof(adc_digi_output_data_t);
    adc_digi_output_data_t *result = (adc_digi_output_data_t *)buffer;
    int mic_buffer_indices[NUM_MIC_CHANNELS] = {0};
    for (int i = 0; i < num_results; i++)
    {
        int channel = result[i].type1.channel;
        int value = result[i].type1.data;
        int mic_idx = -1;
        if (channel == 4)
            mic_idx = 0;
        else if (channel == 5)
            mic_idx = 1;
        else if (channel == 6)
            mic_idx = 2;
        else if (channel == 7)
            mic_idx = 3;
        else if (channel == 0)
            mic_idx = 4;
        if (mic_idx != -1 && mic_buffer_indices[mic_idx] < DMA_BUFFER_SIZE / NUM_MIC_CHANNELS)
        {
            mic_data[mic_idx][mic_buffer_indices[mic_idx]++] = value;
        }
    }

    printf("%d,%d,%d,%d,%d", mic_data[0][0], mic_data[1][1], mic_data[2][2], mic_data[3][3], mic_data[4][4]);
    printf("\n");
    vTaskDelay(1);
}

void adc_acquisition_task(void *arg)
{
    uint32_t bytes_read = 0;
    ESP_ERROR_CHECK(adc_continuous_start(adc_handle));
    while (1)
    {
        esp_err_t err = adc_continuous_read(adc_handle, adc_read_buffer, DMA_BUFFER_SIZE, &bytes_read, 1000);
        if (err == ESP_OK && bytes_read > 0)
        {
            process_adc_samples(adc_read_buffer, bytes_read);
        }
        else if (err != ESP_OK && err != ESP_ERR_TIMEOUT)
        {
            ESP_LOGE(TAG, "Error al leer ADC: %s", esp_err_to_name(err));
        }
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Iniciando ADC continuo...");
    adc_continuous_init();
    xTaskCreate(adc_acquisition_task, "adc_acq_task", 4096, NULL, 5, NULL);
    ESP_LOGI(TAG, "Sistema listo. Adquisición de ADC iniciada.");
}