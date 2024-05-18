from flask import Flask, request
import subprocess
import csv

app = Flask(__name__)

def read_csv(file_name):
    data = []
    with open(file_name, 'r') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            data.append(row)
    return data

@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        ciphertext = request.form['ciphertext']
        if request.form.get('checkbox') == 'on':
            app_to_run = "/usr/local/bin/app2_chi"
            key_length = request.form['key_length']
            file1 = "output_ch.csv"
            file2 = "output_dec.csv"
            title1 = "Key letters based on Chi squared statistics"
            title2 = "Ciphertext deciphered with the found key"
            arguments = [app_to_run, ciphertext, key_length]
        else:
            app_to_run = "/usr/local/bin/app1_kasiski"
            file1 = "output_ngrams.csv"
            file2 = "output_div.csv"
            title1 = "N-grams (3 and 4) and biggest distance between two apperances in ciphertext"
            title2 = "Common divisors (2-24) and count of distance values divisible by them"
            arguments = [app_to_run, ciphertext]
        try:
            result = subprocess.run(arguments, capture_output=True, text=True)
        except subprocess.CalledProcessError as e:
           print(result, file=sys.stderr)
           return result.stderr

        data1 = read_csv(file1)
        data2 = read_csv(file2)

        data_str1 = ''
        for row in data1:
            data_str1 += ', &emsp;'.join(row) + '<br>'

        data_str2 = ''
        for row in data2:
            data_str2 += ', &emsp;'.join(row) + '<br>'

        return f'''
            <html>
            <head>
                <style>
                    .data-table {{
                        width: 100%;
                        word-wrap: break-word;
                    }}
                </style>
            </head>
            <body>
                <h3>{title1}</h3>
                <div class="data-table">
                    {data_str1}
                </div>
                <h3>{title2}</h3>
                <div class="data-table">
                    {data_str2}
                </div>
            </body>
            </html>
        '''

    return '''
        <form method="post">
            <h2>Cryptanalysys helper - Vigenere cipher</h2>
            There are 2 options:<br>
            <h4>+-> kasiski's test (search for key length)</h4>
            <h4>+-> chi-squared statistics (known key length - try to find the key)</h4>

            <p>
            <p><label for="ciphertext">CIPHERTEXT:</label></p>
            <textarea id="ciphertext" name="ciphertext" rows="5" cols="50">
            </textarea><br>
            <p>
            KEY LENGTH: <input type="text" name="key_length"><br>
            <p>
            <input type="checkbox" name="checkbox" value="on">Known key length<br>
            <p>
            <input type="submit" value="Submit">
        </form>
    '''

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
