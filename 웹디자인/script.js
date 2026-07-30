"use strict";

const STORAGE_KEYS = {
  favorites: "winter-fit-finder:favorites",
  memo: "winter-fit-finder:memo",
  preferences: "winter-fit-finder:preferences",
  display: "winter-fit-finder:display"
};

const looks = [
  {
    id: "long-coat",
    title: "롱코트 & 터틀넥 니트",
    subtitle: "단정한 실루엣의 클래식 조합",
    description: "긴 아우터와 목을 감싸는 니트로 보온성과 정돈된 인상을 함께 챙기는 코디입니다.",
    image: "assets/images/long-coat-sweater.png",
    alt: "검은 롱코트와 터틀넥 니트를 활용한 세 가지 코디",
    temperatures: ["freezing", "cold"],
    occasions: ["daily", "office", "date"],
    styles: ["classic", "feminine"],
    warmth: 4,
    tags: ["롱코트", "터틀넥", "출근룩"],
    source: "https://c-writing.aztext.net/entry/%EA%B0%80%EC%9D%84-%ED%8C%A8%EC%85%98-%EC%BD%94%EB%94%94-%EB%B8%94%EB%9E%99-%ED%95%98%EC%9D%B4%EC%9B%A8%EC%9D%B4%EC%8A%A4%ED%8A%B8-%ED%8C%AC%EC%B8%A0%EC%97%90-%ED%84%B0%ED%8B%80%EB%84%A5-%EC%8A%A4%EC%9B%A8%ED%84%B0%EC%99%80-%EB%A1%B1-%EC%BD%94%ED%8A%B8"
  },
  {
    id: "shearling-joggers",
    title: "무스탕 & 기모 조거팬츠",
    subtitle: "활동성과 보온성을 높인 캐주얼 룩",
    description: "두꺼운 무스탕과 기모 팬츠를 조합해 추운 날의 야외 이동에도 편안하도록 구성했습니다.",
    image: "assets/images/shearling-joggers.jpg",
    alt: "밝은색 무스탕과 주황색 조거팬츠를 입은 겨울 코디",
    temperatures: ["freezing", "cold"],
    occasions: ["daily", "outdoor"],
    styles: ["casual"],
    warmth: 5,
    tags: ["무스탕", "조거팬츠", "야외 활동"],
    source: "https://v.daum.net/v/GAkt48D7r4"
  },
  {
    id: "knit-skirt",
    title: "니트 & 롱스커트",
    subtitle: "부드러운 색감의 페미닌 룩",
    description: "레이어드 니트와 롱스커트를 활용해 차분하면서도 포인트가 있는 약속용 코디입니다.",
    image: "assets/images/knit-long-skirt.jpg",
    alt: "아이보리 니트와 체크 롱스커트를 입은 겨울 코디",
    temperatures: ["cold", "cool"],
    occasions: ["office", "date"],
    styles: ["feminine", "classic"],
    warmth: 3,
    tags: ["레이어드 니트", "롱스커트", "데이트룩"],
    source: "https://v.daum.net/v/5bfe5188f3a1d400013884ec"
  },
  {
    id: "cardigan",
    title: "케이블 니트 가디건",
    subtitle: "실내외 온도 차에 대응하는 레이어드",
    description: "이너 위에 쉽게 입고 벗을 수 있어 비교적 온화한 겨울날이나 실내 활동에 적합합니다.",
    image: "assets/images/winter-cardigan.jpg",
    alt: "아이보리 케이블 니트 가디건과 청바지 코디",
    temperatures: ["cold", "cool"],
    occasions: ["daily", "office"],
    styles: ["classic", "casual"],
    warmth: 3,
    tags: ["가디건", "레이어드", "데일리룩"],
    source: "http://www.10x10.co.kr/shopping/category_prd.asp?itemid=3483884"
  },
  {
    id: "accessories",
    type: "accessory",
    title: "겨울 액세서리 레이어링",
    subtitle: "모자·머플러·장갑으로 체감온도 보완",
    description: "기본 코디에 액세서리를 더해 목, 손, 머리의 보온을 보완하고 색상 포인트를 줄 수 있습니다.",
    images: [
      "assets/images/winter-hats.png",
      "assets/images/winter-mufflers.png",
      "assets/images/winter-gloves.png"
    ],
    alt: "겨울 모자, 머플러, 장갑 상품을 모은 이미지",
    temperatures: ["freezing", "cold", "cool"],
    occasions: ["daily", "office", "date", "outdoor"],
    styles: ["classic", "casual", "feminine"],
    warmth: 4,
    tags: ["모자", "머플러", "장갑"],
    source: "https://post.naver.com/viewer/postView.naver?volumeNo=30531703&memberNo=4656129"
  }
];

const labels = {
  temperatures: {
    freezing: "영하권",
    cold: "0~7°C",
    cool: "8~12°C"
  },
  occasions: {
    daily: "일상",
    office: "출근·학교",
    date: "약속·데이트",
    outdoor: "야외 활동"
  },
  styles: {
    classic: "클래식",
    casual: "캐주얼",
    feminine: "페미닌"
  }
};

const preferenceForm = document.querySelector("#preferenceForm");
const recommendationGrid = document.querySelector("#recommendationGrid");
const collectionList = document.querySelector("#collectionList");
const resultSummary = document.querySelector("#resultSummary");
const favoritesOnly = document.querySelector("#favoritesOnly");
const emptyState = document.querySelector("#emptyState");
const resetPreferencesButton = document.querySelector("#resetPreferencesButton");
const memoForm = document.querySelector("#memoForm");
const memoText = document.querySelector("#memoText");
const memoCount = document.querySelector("#memoCount");
const memoStatus = document.querySelector("#memoStatus");
const clearMemoButton = document.querySelector("#clearMemoButton");
const settingsButton = document.querySelector("#settingsButton");
const closeSettingsButton = document.querySelector("#closeSettingsButton");
const settingsPanel = document.querySelector("#settingsPanel");
const panelBackdrop = document.querySelector("#panelBackdrop");
const fontSizeSelect = document.querySelector("#fontSizeSelect");
const contrastToggle = document.querySelector("#contrastToggle");

let favorites = new Set(readStorage(STORAGE_KEYS.favorites, []));
let currentRankedLooks = [];

function readStorage(key, fallback) {
  try {
    const value = window.localStorage.getItem(key);
    return value ? JSON.parse(value) : fallback;
  } catch (error) {
    console.warn(`LocalStorage 읽기 실패: ${key}`, error);
    return fallback;
  }
}

function writeStorage(key, value) {
  try {
    window.localStorage.setItem(key, JSON.stringify(value));
    return true;
  } catch (error) {
    console.warn(`LocalStorage 저장 실패: ${key}`, error);
    return false;
  }
}

function getPreferences() {
  const formData = new FormData(preferenceForm);
  return {
    temperature: formData.get("temperature"),
    occasion: formData.get("occasion"),
    style: formData.get("style"),
    warmthPriority: formData.get("warmthPriority") === "on"
  };
}

function scoreLook(look, preferences) {
  let score = 0;
  const reasons = [];

  // 액세서리는 완성 코디와 같은 기준으로 1위를 차지하지 않도록
  // 보온을 보완하는 보조 항목으로 별도 점수화합니다.
  if (look.type === "accessory") {
    if (look.temperatures.includes(preferences.temperature)) {
      score += 2;
      reasons.push(`${labels.temperatures[preferences.temperature]} 체감온도 보완`);
    }
    if (look.occasions.includes(preferences.occasion)) score += 1;
    if (look.styles.includes(preferences.style)) score += 1;
    if (preferences.warmthPriority) {
      score += 2;
      reasons.push("기본 코디에 보온 아이템 추가");
    }
    return { ...look, score, reasons };
  }

  if (look.temperatures.includes(preferences.temperature)) {
    score += 4;
    reasons.push(`${labels.temperatures[preferences.temperature]} 기온에 적합`);
  } else if (
    (preferences.temperature === "freezing" && look.temperatures.includes("cold")) ||
    (preferences.temperature === "cool" && look.temperatures.includes("cold"))
  ) {
    score += 1;
    reasons.push("레이어드 조절 가능");
  }

  if (look.occasions.includes(preferences.occasion)) {
    score += 3;
    reasons.push(`${labels.occasions[preferences.occasion]} 일정과 잘 맞음`);
  }

  if (look.styles.includes(preferences.style)) {
    score += 3;
    reasons.push(`${labels.styles[preferences.style]} 취향 반영`);
  }

  if (preferences.warmthPriority) {
    const warmthBonus = look.warmth >= 4 ? 2 : look.warmth === 3 ? 1 : 0;
    score += warmthBonus;
    if (warmthBonus > 0) reasons.push(`보온도 ${look.warmth}/5`);
  }

  return { ...look, score, reasons };
}

function rankLooks(preferences) {
  return looks
    .map((look) => scoreLook(look, preferences))
    .sort((a, b) => b.score - a.score || b.warmth - a.warmth || a.title.localeCompare(b.title, "ko"));
}

function createMedia(look) {
  if (look.images) {
    return `
      <div class="look-card__media look-card__media--collage" role="img" aria-label="${look.alt}">
        ${look.images.map((src) => `<img src="${src}" alt="" loading="lazy">`).join("")}
      </div>
    `;
  }

  return `
    <div class="look-card__media">
      <img src="${look.image}" alt="${look.alt}" loading="lazy">
    </div>
  `;
}

function createLookCard(look, rank) {
  const favorite = favorites.has(look.id);
  const rankLabel = rank === 1 ? "BEST" : `TOP ${rank}`;
  const reasonText = look.reasons.length ? look.reasons.slice(0, 3).join(" · ") : "선택 조건과의 일치 항목이 적어요.";

  return `
    <article class="look-card" data-look-id="${look.id}">
      <div class="rank-badge">${rankLabel}</div>
      <button
        class="favorite-button"
        type="button"
        data-favorite-id="${look.id}"
        aria-pressed="${favorite}"
        aria-label="${look.title} ${favorite ? "저장 취소" : "저장"}"
      >${favorite ? "♥" : "♡"}</button>
      ${createMedia(look)}
      <div class="look-card__body">
        <h3>${look.title}</h3>
        <p>${reasonText}</p>
        <div class="score-row">
          <span>적합도 점수</span>
          <strong class="score-value">${look.score}<small> / 12</small></strong>
        </div>
        <ul class="tag-list" aria-label="코디 키워드">
          ${look.tags.map((tag) => `<li>${tag}</li>`).join("")}
        </ul>
      </div>
    </article>
  `;
}

function renderRecommendations() {
  const visibleLooks = favoritesOnly.checked
    ? currentRankedLooks.filter((look) => favorites.has(look.id))
    : currentRankedLooks.slice(0, 3);

  recommendationGrid.innerHTML = visibleLooks
    .map((look) => createLookCard(look, currentRankedLooks.indexOf(look) + 1))
    .join("");

  emptyState.hidden = visibleLooks.length > 0;
  recommendationGrid.hidden = visibleLooks.length === 0;
}

function createCollectionItem(look) {
  const image = look.images ? look.images[0] : look.image;
  return `
    <article class="collection-item">
      <div class="collection-item__image">
        <img src="${image}" alt="${look.alt}" loading="lazy">
      </div>
      <div>
        <h3>${look.title}</h3>
        <p>${look.description}</p>
        <ul class="tag-list" aria-label="코디 키워드">
          ${look.tags.map((tag) => `<li>${tag}</li>`).join("")}
        </ul>
        <a class="source-link" href="${look.source}" target="_blank" rel="noopener noreferrer">이미지 출처 보기 ↗</a>
      </div>
      <div class="warmth-meter" aria-label="보온도 ${look.warmth}점, 5점 만점">
        <strong>보온도</strong>
        <span aria-hidden="true">${"●".repeat(look.warmth)}${"○".repeat(5 - look.warmth)}</span>
      </div>
    </article>
  `;
}

function renderCollection() {
  collectionList.innerHTML = looks.map(createCollectionItem).join("");
}

function applyPreferences(preferences, announce = true) {
  currentRankedLooks = rankLooks(preferences);
  writeStorage(STORAGE_KEYS.preferences, preferences);
  favoritesOnly.checked = false;
  renderRecommendations();

  if (announce) {
    const top = currentRankedLooks[0];
    resultSummary.textContent = `${labels.temperatures[preferences.temperature]} · ${labels.occasions[preferences.occasion]} · ${labels.styles[preferences.style]} 조건에서 “${top.title}”의 점수가 가장 높아요.`;
    document.querySelector("#resultsTitle").scrollIntoView({ behavior: "smooth", block: "start" });
  }
}

function setFormPreferences(preferences) {
  if (!preferences) return;

  ["temperature", "occasion", "style"].forEach((name) => {
    const value = preferences[name];
    const input = preferenceForm.querySelector(`[name="${name}"][value="${value}"]`);
    if (input) input.checked = true;
  });

  preferenceForm.elements.warmthPriority.checked = Boolean(preferences.warmthPriority);
}

function toggleFavorite(id) {
  if (favorites.has(id)) {
    favorites.delete(id);
  } else {
    favorites.add(id);
  }
  writeStorage(STORAGE_KEYS.favorites, [...favorites]);
  renderRecommendations();
}

function updateMemoCount() {
  memoCount.textContent = `${memoText.value.length} / 300`;
}

function loadMemo() {
  const savedMemo = readStorage(STORAGE_KEYS.memo, "");
  memoText.value = typeof savedMemo === "string" ? savedMemo : "";
  updateMemoCount();
}

function openSettings() {
  settingsPanel.hidden = false;
  panelBackdrop.hidden = false;
  document.body.classList.add("panel-open");
  settingsButton.setAttribute("aria-expanded", "true");
  closeSettingsButton.focus();
}

function closeSettings() {
  settingsPanel.hidden = true;
  panelBackdrop.hidden = true;
  document.body.classList.remove("panel-open");
  settingsButton.setAttribute("aria-expanded", "false");
  settingsButton.focus();
}

function applyDisplaySettings(settings) {
  const fontSize = { small: "14.72px", normal: "16px", large: "17.92px" }[settings.fontSize] || "16px";
  document.documentElement.style.fontSize = fontSize;
  document.body.classList.toggle("high-contrast", Boolean(settings.highContrast));
  fontSizeSelect.value = settings.fontSize || "normal";
  contrastToggle.checked = Boolean(settings.highContrast);
}

function saveDisplaySettings() {
  const settings = {
    fontSize: fontSizeSelect.value,
    highContrast: contrastToggle.checked
  };
  writeStorage(STORAGE_KEYS.display, settings);
  applyDisplaySettings(settings);
}

preferenceForm.addEventListener("submit", (event) => {
  event.preventDefault();
  applyPreferences(getPreferences());
});

resetPreferencesButton.addEventListener("click", () => {
  preferenceForm.reset();
  const defaults = getPreferences();
  applyPreferences(defaults, false);
  resultSummary.textContent = "선택을 초기화하고 기본 조건으로 돌아왔어요.";
});

recommendationGrid.addEventListener("click", (event) => {
  const button = event.target.closest("[data-favorite-id]");
  if (button) toggleFavorite(button.dataset.favoriteId);
});

favoritesOnly.addEventListener("change", renderRecommendations);

memoText.addEventListener("input", updateMemoCount);

memoForm.addEventListener("submit", (event) => {
  event.preventDefault();
  const saved = writeStorage(STORAGE_KEYS.memo, memoText.value.trim());
  memoStatus.textContent = saved ? "저장했어요." : "저장하지 못했어요.";
  window.setTimeout(() => {
    memoStatus.textContent = "";
  }, 2200);
});

clearMemoButton.addEventListener("click", () => {
  memoText.value = "";
  writeStorage(STORAGE_KEYS.memo, "");
  updateMemoCount();
  memoStatus.textContent = "삭제했어요.";
});

settingsButton.addEventListener("click", openSettings);
closeSettingsButton.addEventListener("click", closeSettings);
panelBackdrop.addEventListener("click", closeSettings);

document.addEventListener("keydown", (event) => {
  if (event.key === "Escape" && !settingsPanel.hidden) closeSettings();
});

fontSizeSelect.addEventListener("change", saveDisplaySettings);
contrastToggle.addEventListener("change", saveDisplaySettings);

function initialize() {
  const savedPreferences = readStorage(STORAGE_KEYS.preferences, null);
  setFormPreferences(savedPreferences);
  applyPreferences(getPreferences(), false);
  renderCollection();
  loadMemo();
  applyDisplaySettings(readStorage(STORAGE_KEYS.display, { fontSize: "normal", highContrast: false }));
}

initialize();
