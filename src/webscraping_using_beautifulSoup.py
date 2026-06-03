import requests
from bs4 import BeautifulSoup
import blynklib
import schedule
import time


BLYNK_AUTH = "gz31KOh7qmT3sGrdjTYcekOPutEinZKX"


# URL of the Keells Super website
url = "https://www.keellssuper.com/productDetail?itemcode=8710&Brown_Sugar_Bulk_kg"

# Fetch the page
response = requests.get(url)
if response.status_code == 200:
    soup = BeautifulSoup(response.content, 'html.parser')

    # Example: Extract product names and prices
    products = soup.find_all('div', class_="productName col")  # Adjust based on the website's structure
    prices = soup.find_all('div', class_= "product-card-final-price")    # Adjust based on the website's structure

    for product, price in zip(products, prices):
        print(f"{product.text.strip()} - {price.text.strip()}")
else:
    print(f"Failed to fetch the website. Status Code: {response.status_code}")



def update_blynk_data():
    
# Initialize Blynk
blynk = blynklib.Blynk(BLYNK_AUTH)

# Data to send 
product_data = "{product.text.strip()} - {price.text.strip()}"

# Virtual Pin to send the data
virtual_pin = 'V1'

# Push data to Blynk
blynk.virtual_write(virtual_pin, product_data)

    pass

# Schedule the task every hour
schedule.every(1).hours.do(update_blynk_data)

while True:
    schedule.run_pending()
    time.sleep(1)

