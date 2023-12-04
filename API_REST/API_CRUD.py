from flask import Flask, jsonify, request, render_template, abort

app = Flask(__name__)

welcome = "Welcome to 3ESE API!"

@app.route('/api/welcome/', methods=['GET'])
def api_get_welcome():
    return jsonify({'message': welcome}), 200
@app.route('/api/welcome/<int:index>', methods=['GET'])
def api_get_welcome_index(index):
    if index < 0 or index >= len(welcome):
        abort(404)
    return jsonify({'letter': welcome[index-1]}), 200

@app.route('/api/welcome/', methods=['POST'])
def api_create_welcome():
    replace_welcome = request.get_json()
    new_welcome = replace_welcome.get('message', '')
    global welcome
    welcome = new_welcome
    return jsonify({'message': welcome}), 202

@app.route('/api/welcome/<int:index>', methods=['PUT'])
def api_update_welcome(index):
    add_word = request.get_json()
    new_word = add_word.get('word', '')
    global welcome
    welcome = welcome[:index] + new_word + welcome[index+len(new_word):]
    return jsonify({'message': welcome}), 200

@app.route('/api/welcome/<int:index>', methods=['PATCH'])
def api_patch_welcome(index):
    replace_letter = request.get_json()
    new_letter = replace_letter.get('letter', '')
    global welcome
    welcome = welcome[:index-1] + new_letter + welcome[index:]
    return jsonify({'message': welcome}), 200

@app.route('/api/welcome/<int:index>', methods=['DELETE'])
def api_delete_welcome_index(index):
    global welcome
    welcome = welcome[:index-1] + welcome[index:] 
    return jsonify({'message': welcome}), 200

@app.route('/api/welcome/', methods=['DELETE'])
def api_delete_welcome():
    global welcome
    welcome = ''
    return jsonify({'message': welcome}), 200
# ERREUR 404 si @app.route('/')
@app.errorhandler(404)
def page_not_found(error):
    return render_template('page_not_found.html'), 404
# à décommenter si cela ne fonctionne pas
if __name__ == "__main__":
    app.run(debug=True)