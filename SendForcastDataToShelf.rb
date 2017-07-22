require 'net/http'
require 'json'
require 'rubyserial'

TSTORM_STRS = ['tstorms', 'chancetstorms']
RAIN_STRS = ['chancerain', 'rain']
SNOWY_STRS = ['chanceflurries','chancesleet','chancesnow','flurries','sleet','snow']
CLOUDY_STRS = ['cloudy']
MOSTLYCLOUDY_STRS = ['mostlycloudy','fog']
PARTLYCLOUDY_STRS = ['hazy','partlycloudy','partlysunny']
SUNNY_STRS = ['sunny','clear','mostlysunny','unknown']

def today_string(data)
  string = ''
  gust = data['wind_gust_mph']
  icon = data['icon']
  average_wind = data['wind_mph']

  string += '1' if TSTORM_STRS.include?(icon)
  string += '2' if RAIN_STRS.include?(icon)
  string += '3' if SNOWY_STRS.include?(icon)
  string += '4' if CLOUDY_STRS.include?(icon)
  string += '5' if MOSTLYCLOUDY_STRS.include?(icon)
  string += '6' if PARTLYCLOUDY_STRS.include?(icon)
  string += '7' if SUNNY_STRS.include?(icon)
  raise 'EFF' if string.empty?

  string += '1' if average_wind <= 9
  string += '2' if average_wind > 9 && average_wind <= 19
  string += '3' if average_wind > 19 && average_wind <= 29
  string += '4' if average_wind > 29
  raise 'EFF2' if string.length != 2

  string += '1' if gust <= 9
  string += '2' if gust > 9 && gust <= 19
  string += '3' if gust > 19 && gust <= 29
  string += '4' if gust > 29
  raise 'EFF2' if string.length != 3

  string
end

def day_string(data)
  string = ''
  gust = data['maxwind']['mph']
  icon = data['icon']
  average_wind = data['avewind']['mph']
  pop = data['pop']

  string += '1' if TSTORM_STRS.include?(icon)
  string += '2' if RAIN_STRS.include?(icon)
  string += '3' if SNOWY_STRS.include?(icon)
  string += '4' if CLOUDY_STRS.include?(icon)
  string += '5' if MOSTLYCLOUDY_STRS.include?(icon)
  string += '6' if PARTLYCLOUDY_STRS.include?(icon)
  string += '7' if SUNNY_STRS.include?(icon)

  if icon.include?('chance') && (TSTORM_STRS.include?(icon) || RAIN_STRS.include?(icon) || SNOWY_STRS.include?(icon))
    string = '4' if pop <= 60
  end

  raise 'EFF' if string.empty?
  string += '1' if average_wind <= 9
  string += '2' if average_wind > 9 && average_wind <= 19
  string += '3' if average_wind > 19 && average_wind <= 29
  string += '4' if average_wind > 29
  raise 'EFF2' if string.length != 2

  string += '1' if gust <= 9
  string += '2' if gust > 9 && gust <= 19
  string += '3' if gust > 19 && gust <= 29
  string += '4' if gust > 29
  raise 'EFF3' if string.length != 3

  string
end

ZIP = 11106
TOKEN = 'CHANGME'
BASE_URL = "http://api.wunderground.com/api/#{TOKEN}"

forecast_url = "#{BASE_URL}/forecast/q/#{ZIP}.json"
forecast_response = JSON.parse(Net::HTTP.get(URI(forecast_url)))

today_url = "#{BASE_URL}/conditions/q/#{ZIP}.json"
today_response = JSON.parse(Net::HTTP.get(URI(today_url)))

today_data = today_response['current_observation']
one_data = forecast_response['forecast']['simpleforecast']['forecastday'][0]
two_data = forecast_response['forecast']['simpleforecast']['forecastday'][1]
three_data = forecast_response['forecast']['simpleforecast']['forecastday'][2]

today_conditions = today_string(today_data)
one_conditions = day_string(one_data)
two_conditions = day_string(two_data)
three_conditions = day_string(three_data)

puts new_data = "##{Time.now.to_i}#<#{today_conditions + one_conditions + two_conditions + three_conditions}>"

port_str = 'COM3'
# baud_rate = 9600
# data_bits = 8
# stop_bits = 1
# parity = SerialPort::NONE
sp = Serial.new(port_str)
sp.write(new_data)

