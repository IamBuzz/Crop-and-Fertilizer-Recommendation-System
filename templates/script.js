
      // OpenWeatherMap API key
      const apiKey = '68dbaad401564a4384995403231512';

      // Function to fetch weather data for a given district
      async function fetchWeatherData(district) {
        const apiUrl = `https://api.openweathermap.org/data/2.5/weather?q=${district}&appid=${apiKey}&units=metric`;
        const response = await fetch(apiUrl);
        const data = await response.json();
        return data.main.temp;
      }
      const districtData = new Map([
        ['Kolhapur', 1000],
        ['Pune', 1000],
        ['Satara', 1000],
        ['Sangli', 800],
        ['Solapur', 900]
      ]);

      // Get the select element and input field
      const districtSelect = document.getElementById('district');
      const rainfall = document.getElementById('rainfall');

      // Add event listener to select element
      districtSelect.addEventListener('change', function() {
        const selectedDistrict = this.value;
        const rainfallOutput = districtData.get(selectedDistrict);
        rainfall.value = rainfallOutput;
        try {
          const temperature = await fetchWeatherData(selectedDistrict);
          temperatureInput.value = temperature;
        } catch (error) {
            console.error('Error fetching weather data:', error);
        }
      });z