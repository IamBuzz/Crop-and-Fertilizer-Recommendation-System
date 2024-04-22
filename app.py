from flask import Flask, render_template, request
import pickle

app = Flask(__name__)

# Load the trained model from the pickle file
with open('model.pkl', 'rb') as f:
    model = pickle.load(f)
with open('modelCrop.pkl', 'rb') as f:
    modelCrop = pickle.load(f)

# Function to make predictions
def predict(nitrogen, phosphorus, potassium, ph, rainfall, temperature):
    # Perform some processing and make predictions using the loaded model
    prediction = model.predict([[nitrogen, phosphorus, potassium, ph, rainfall, temperature]])
    return prediction

def predictCrop(nitrogen, phosphorus, potassium, ph, rainfall, temperature):
    # Perform some processing and make predictions using the loaded model
    prediction = modelCrop.predict([[nitrogen, phosphorus, potassium, ph, rainfall, temperature]])
    return prediction

# Route to serve the HTML file
@app.route('/')
def index():
    return render_template('index.html')

# Flask route handler
@app.route('/predict', methods=['POST'])
def predict_crop():
    # Get the data from the request
    nitrogen = float(request.form['nitrogen'])
    phosphorus = float(request.form['phosphorus'])
    potassium = float(request.form['potassium'])
    temperature = float(request.form['temperature'])
    rainfall = float(request.form['rainfall'])
    ph = float(request.form['ph'])

    # Make prediction
    prediction = predict(nitrogen, phosphorus, potassium, ph, rainfall, temperature)
    predictC = predictCrop(nitrogen, phosphorus, potassium, ph, rainfall, temperature)
    fer = ' '.join(prediction)
    cp = ' '.join(predictC)

    # Return prediction as HTML response
    return render_template('index.html', fertilizer=fer, crop=cp)

if __name__ == '__main__':
    app.run(debug=True)
