<template>
  <div class="detective-app">
    <!-- Header -->
    <header>
      <h1>🔍 DEFINITION DETECTIVE</h1>
      <p class="subtitle">"Uncover the meaning of words"</p>
    </header>

    <!-- Search Area -->
    <div class="search-area">
      <div class="search-box">
        <input 
          v-model="searchTerm" 
          @keyup.enter="searchDefinition"
          placeholder="Type a word to investigate..."
          type="text"
          ref="searchInput"
        />
        <button @click="searchDefinition" :disabled="!searchTerm.trim() || isLoading">
          <span v-if="!isLoading">🔎 Investigate</span>
          <span v-else>⏳ Searching...</span>
        </button>
      </div>
      
      <!-- Quick word suggestions -->
      <div class="quick-words">
        <span>Quick words:</span>
        <button 
          v-for="word in quickWords" 
          :key="word"
          @click="searchWord(word)"
          class="quick-btn"
        >
          {{ word }}
        </button>
      </div>
    </div>

    <!-- Results Area -->
    <div v-if="definition" class="results">
      <div class="word-card">
        <div class="word-header">
          <h2>{{ definition.word }}</h2>
          <span v-if="definition.phonetic" class="phonetic">
            /{{ definition.phonetic }}/
          </span>
          <button @click="speakWord" class="speak-btn" title="Listen to pronunciation">
            🔊
          </button>
        </div>

        <!-- Definitions -->
        <div class="definitions">
          <div v-for="(meaning, index) in definition.meanings" :key="index" class="meaning-group">
            <h3>{{ meaning.partOfSpeech }}</h3>
            <ul>
              <li v-for="(def, idx) in meaning.definitions" :key="idx">
                {{ def.definition }}
                <span v-if="def.example" class="example">"{{ def.example }}"</span>
              </li>
            </ul>
          </div>
        </div>

        <!-- Synonyms -->
        <div v-if="hasSynonyms" class="synonyms">
          <h3>🔗 Similar Words</h3>
          <div class="synonym-tags">
            <span 
              v-for="synonym in allSynonyms" 
              :key="synonym"
              @click="searchWord(synonym)"
              class="synonym-tag"
            >
              {{ synonym }}
            </span>
          </div>
        </div>
      </div>
    </div>

    <!-- Error State -->
    <div v-if="error" class="error">
      <p>❌ {{ error }}</p>
      <p class="error-hint">Try checking the spelling or try another word!</p>
    </div>

    <!-- Empty State -->
    <div v-if="!definition && !error && !isLoading" class="empty-state">
      <p>🕵️ Enter a word to start your investigation...</p>
      <div class="keyboard">
        <div v-for="row in keyboardLayout" :key="row.id" class="keyboard-row">
          <span 
            v-for="key in row.keys" 
            :key="key"
            class="key"
            @click="addToSearch(key)"
          >
            {{ key }}
          </span>
        </div>
      </div>
    </div>

    <!-- Loading State -->
    <div v-if="isLoading" class="loading">
      <div class="spinner"></div>
      <p>Investigating...</p>
    </div>

    <!-- Footer -->
    <footer>
      <p>© 2026 - OUAHIDI AGENCY | Powered by Free Dictionary API</p>
    </footer>
  </div>
</template>

<script setup>
import { ref, computed } from 'vue';

// --- STATE ---
const searchTerm = ref('');
const definition = ref(null);
const error = ref(null);
const isLoading = ref(false);
const searchInput = ref(null);

// --- QUICK WORDS ---
const quickWords = ['detective', 'investigate', 'mystery', 'clue', 'evidence', 'alibi'];

// --- KEYBOARD LAYOUT ---
const keyboardLayout = [
  { id: 1, keys: ['Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'] },
  { id: 2, keys: ['A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'] },
  { id: 3, keys: ['Z', 'X', 'C', 'V', 'B', 'N', 'M'] }
];

// --- COMPUTED ---
const hasSynonyms = computed(() => {
  return definition.value?.meanings?.some(m => m.definitions?.some(d => d.synonyms?.length > 0));
});

const allSynonyms = computed(() => {
  if (!definition.value) return [];
  const synonyms = [];
  definition.value.meanings.forEach(meaning => {
    meaning.definitions.forEach(def => {
      if (def.synonyms) {
        synonyms.push(...def.synonyms);
      }
    });
  });
  return [...new Set(synonyms)].slice(0, 10); // Remove duplicates and limit to 10
});

// --- METHODS ---
const searchDefinition = async () => {
  const term = searchTerm.value.trim();
  if (!term) return;
  
  // Reset states
  error.value = null;
  definition.value = null;
  isLoading.value = true;
  
  try {
    const response = await fetch(`https://api.dictionaryapi.dev/api/v2/entries/en/${encodeURIComponent(term)}`);
    
    if (!response.ok) {
      if (response.status === 404) {
        throw new Error(`"${term}" not found. Please check your spelling.`);
      }
      throw new Error('Something went wrong. Please try again.');
    }
    
    const data = await response.json();
    
    if (data && data.length > 0) {
      definition.value = data[0];
    } else {
      throw new Error(`No definition found for "${term}"`);
    }
  } catch (err) {
    error.value = err.message;
    definition.value = null;
  } finally {
    isLoading.value = false;
  }
};

const searchWord = (word) => {
  searchTerm.value = word;
  searchDefinition();
};

const addToSearch = (key) => {
  searchTerm.value += key.toLowerCase();
  searchInput.value?.focus();
};

const speakWord = () => {
  if (!definition.value?.word) return;
  const utterance = new SpeechSynthesisUtterance(definition.value.word);
  utterance.lang = 'en-US';
  utterance.rate = 0.8;
  window.speechSynthesis.speak(utterance);
};

// Auto-search on mount
searchDefinition();
</script>

<style scoped>
.detective-app {
  max-width: 800px;
  margin: 0 auto;
  padding: 20px;
  min-height: 100vh;
  background: linear-gradient(135deg, #1a1a2e 0%, #16213e 50%, #0f3460 100%);
  color: #e0e0e0;
  font-family: 'Courier New', monospace;
}

/* HEADER */
header {
  text-align: center;
  padding: 30px 0;
  border-bottom: 2px solid #c9a84c;
  margin-bottom: 30px;
}

header h1 {
  font-size: 2.5rem;
  color: #c9a84c;
  text-shadow: 0 0 20px rgba(201, 168, 76, 0.3);
  letter-spacing: 4px;
  margin: 0;
}

.subtitle {
  color: #888;
  font-style: italic;
  margin-top: 5px;
  font-size: 0.9rem;
}

/* SEARCH AREA */
.search-area {
  margin-bottom: 30px;
}

.search-box {
  display: flex;
  gap: 10px;
  margin-bottom: 15px;
}

.search-box input {
  flex: 1;
  padding: 15px 20px;
  border: 2px solid #c9a84c;
  border-radius: 8px;
  background: rgba(255, 255, 255, 0.05);
  color: white;
  font-size: 1.1rem;
  font-family: 'Courier New', monospace;
  transition: all 0.3s;
}

.search-box input:focus {
  outline: none;
  border-color: #ffd700;
  box-shadow: 0 0 20px rgba(201, 168, 76, 0.2);
  background: rgba(255, 255, 255, 0.1);
}

.search-box input::placeholder {
  color: #666;
}

.search-box button {
  padding: 15px 30px;
  background: linear-gradient(135deg, #c9a84c, #ffd700);
  color: #1a1a2e;
  border: none;
  border-radius: 8px;
  font-weight: bold;
  font-size: 1rem;
  cursor: pointer;
  transition: all 0.3s;
  font-family: 'Courier New', monospace;
  white-space: nowrap;
}

.search-box button:hover:not(:disabled) {
  transform: scale(1.05);
  box-shadow: 0 5px 25px rgba(201, 168, 76, 0.4);
}

.search-box button:disabled {
  opacity: 0.5;
  cursor: not-allowed;
}

/* QUICK WORDS */
.quick-words {
  display: flex;
  gap: 10px;
  align-items: center;
  flex-wrap: wrap;
}

.quick-words span {
  color: #888;
  font-size: 0.9rem;
}

.quick-btn {
  padding: 5px 15px;
  background: rgba(201, 168, 76, 0.1);
  border: 1px solid #c9a84c;
  border-radius: 20px;
  color: #c9a84c;
  cursor: pointer;
  transition: all 0.3s;
  font-family: 'Courier New', monospace;
  font-size: 0.9rem;
}

.quick-btn:hover {
  background: #c9a84c;
  color: #1a1a2e;
  transform: scale(1.05);
}

/* RESULTS */
.results {
  animation: fadeIn 0.5s ease;
}

@keyframes fadeIn {
  from { opacity: 0; transform: translateY(20px); }
  to { opacity: 1; transform: translateY(0); }
}

.word-card {
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid rgba(201, 168, 76, 0.3);
  border-radius: 15px;
  padding: 30px;
  backdrop-filter: blur(10px);
}

.word-header {
  display: flex;
  align-items: center;
  gap: 15px;
  margin-bottom: 25px;
  flex-wrap: wrap;
}

.word-header h2 {
  font-size: 2.5rem;
  color: #c9a84c;
  margin: 0;
  text-transform: capitalize;
}

.phonetic {
  color: #888;
  font-size: 1.1rem;
}

.speak-btn {
  padding: 8px 15px;
  background: rgba(201, 168, 76, 0.2);
  border: 1px solid #c9a84c;
  border-radius: 8px;
  color: #c9a84c;
  cursor: pointer;
  font-size: 1.2rem;
  transition: all 0.3s;
}

.speak-btn:hover {
  background: #c9a84c;
  color: #1a1a2e;
  transform: scale(1.1);
}

/* DEFINITIONS */
.meaning-group {
  margin-bottom: 20px;
  padding-bottom: 20px;
  border-bottom: 1px solid rgba(255, 255, 255, 0.05);
}

.meaning-group:last-child {
  border-bottom: none;
}

.meaning-group h3 {
  color: #ffd700;
  text-transform: uppercase;
  font-size: 0.9rem;
  letter-spacing: 2px;
  margin-bottom: 10px;
}

.meaning-group ul {
  list-style: none;
  padding: 0;
}

.meaning-group li {
  padding: 8px 0;
  padding-left: 20px;
  border-left: 2px solid rgba(201, 168, 76, 0.3);
  margin-bottom: 5px;
  line-height: 1.6;
}

.example {
  display: block;
  color: #888;
  font-style: italic;
  margin-top: 5px;
  padding-left: 20px;
  font-size: 0.95rem;
}

/* SYNONYMS */
.synonyms {
  margin-top: 25px;
  padding-top: 25px;
  border-top: 2px solid rgba(201, 168, 76, 0.2);
}

.synonyms h3 {
  color: #c9a84c;
  margin-bottom: 15px;
}

.synonym-tags {
  display: flex;
  flex-wrap: wrap;
  gap: 10px;
}

.synonym-tag {
  padding: 8px 18px;
  background: rgba(201, 168, 76, 0.1);
  border: 1px solid rgba(201, 168, 76, 0.3);
  border-radius: 20px;
  color: #c9a84c;
  cursor: pointer;
  transition: all 0.3s;
  font-size: 0.9rem;
}

.synonym-tag:hover {
  background: #c9a84c;
  color: #1a1a2e;
  transform: scale(1.05);
}

/* KEYBOARD */
.keyboard {
  margin: 30px 0;
  padding: 20px;
  background: rgba(0, 0, 0, 0.3);
  border-radius: 12px;
  border: 1px solid rgba(201, 168, 76, 0.1);
}

.keyboard-row {
  display: flex;
  justify-content: center;
  gap: 8px;
  margin-bottom: 8px;
}

.keyboard-row:last-child {
  margin-bottom: 0;
}

.key {
  padding: 10px 15px;
  background: rgba(255, 255, 255, 0.05);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 6px;
  color: #888;
  cursor: pointer;
  transition: all 0.2s;
  font-family: 'Courier New', monospace;
  min-width: 40px;
  text-align: center;
  font-size: 0.9rem;
}

.key:hover {
  background: rgba(201, 168, 76, 0.2);
  border-color: #c9a84c;
  color: #c9a84c;
  transform: translateY(-2px);
}

/* EMPTY STATE */
.empty-state {
  text-align: center;
  padding: 40px 0;
}

.empty-state p {
  font-size: 1.2rem;
  color: #888;
  margin-bottom: 20px;
}

/* ERROR */
.error {
  background: rgba(255, 0, 0, 0.1);
  border: 1px solid rgba(255, 0, 0, 0.3);
  border-radius: 12px;
  padding: 20px;
  text-align: center;
  margin: 20px 0;
}

.error p {
  color: #ff6b6b;
  margin: 5px 0;
}

.error-hint {
  color: #888 !important;
  font-size: 0.9rem;
}

/* LOADING */
.loading {
  text-align: center;
  padding: 40px 0;
}

.spinner {
  width: 50px;
  height: 50px;
  border: 3px solid rgba(201, 168, 76, 0.1);
  border-top: 3px solid #c9a84c;
  border-radius: 50%;
  animation: spin 1s linear infinite;
  margin: 0 auto 20px;
}

@keyframes spin {
  0% { transform: rotate(0deg); }
  100% { transform: rotate(360deg); }
}

.loading p {
  color: #888;
}

/* FOOTER */
footer {
  margin-top: 40px;
  padding-top: 20px;
  border-top: 1px solid rgba(255, 255, 255, 0.05);
  text-align: center;
  color: #666;
  font-size: 0.8rem;
}

/* RESPONSIVE */
@media (max-width: 768px) {
  .search-box {
    flex-direction: column;
  }
  
  .search-box button {
    width: 100%;
  }
  
  .word-header h2 {
    font-size: 2rem;
  }
  
  .key {
    min-width: 30px;
    padding: 8px 10px;
    font-size: 0.8rem;
  }
}

@media (max-width: 480px) {
  .key {
    min-width: 25px;
    padding: 6px 8px;
    font-size: 0.7rem;
  }
  
  .keyboard-row {
    gap: 4px;
  }
}
</style>