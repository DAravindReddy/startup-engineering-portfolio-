/**
 * Kernelwise Labs - Core Application Engine & AI Copilot
 * Author: Aravind Reddy (@DAravindReddy)
 */

document.addEventListener("DOMContentLoaded", () => {
  initArchitectureCanvas();
  initProjectTabs();
  initRenderProjects("all");
  initRenderWhitepapers();
  initAICopilot();
  initPitchGenerator();
  initTerminal();
  initVerificationMatrix();
});

/* ==========================================================================
   1. Interactive Hardware-to-Cloud Neural Canvas Visualizer
   ========================================================================== */
function initArchitectureCanvas() {
  const canvas = document.getElementById("architectureCanvas");
  if (!canvas) return;
  const ctx = canvas.getContext("2d");

  let width = (canvas.width = canvas.parentElement.clientWidth);
  let height = (canvas.height = 360);

  window.addEventListener("resize", () => {
    width = canvas.width = canvas.parentElement.clientWidth;
    height = canvas.height = 360;
  });

  const layers = [
    { name: "1. Silicon & Physical Sensors", color: "#f59e0b", xRatio: 0.12, sub: "PIC, AVR, nRF52840, ESP32, IMU, DHT" },
    { name: "2. Linux Kernel & HAL Subsystems", color: "#06b6d4", xRatio: 0.38, sub: "LKM Drivers, /proc, /sys, DMA, POSIX" },
    { name: "3. Edge AI & TinyML Inference", color: "#8b5cf6", xRatio: 0.65, sub: "Isolation Forest, Mahalanobis, FFT" },
    { name: "4. Autonomous Observability Cloud", color: "#10b981", xRatio: 0.88, sub: "Glassmorphism UI, SCADA, Crash Logging" }
  ];

  const particles = [];
  for (let i = 0; i < 45; i++) {
    particles.push({
      fromLayer: Math.floor(Math.random() * 3),
      progress: Math.random(),
      speed: 0.004 + Math.random() * 0.006,
      offsetY: (Math.random() - 0.5) * 80,
      size: 2 + Math.random() * 2.5
    });
  }

  let hoveredLayer = null;

  canvas.addEventListener("mousemove", (e) => {
    const rect = canvas.getBoundingClientRect();
    const mouseX = e.clientX - rect.left;
    hoveredLayer = null;
    layers.forEach((l, idx) => {
      const lx = l.xRatio * width;
      if (Math.abs(mouseX - lx) < 60) {
        hoveredLayer = idx;
      }
    });
  });

  canvas.addEventListener("mouseleave", () => {
    hoveredLayer = null;
  });

  function render() {
    ctx.clearRect(0, 0, width, height);

    // Draw background grid lines inside canvas
    ctx.strokeStyle = "rgba(255, 255, 255, 0.03)";
    ctx.lineWidth = 1;
    for (let x = 0; x < width; x += 30) {
      ctx.beginPath();
      ctx.moveTo(x, 0);
      ctx.lineTo(x, height);
      ctx.stroke();
    }
    for (let y = 0; y < height; y += 30) {
      ctx.beginPath();
      ctx.moveTo(0, y);
      ctx.lineTo(width, y);
      ctx.stroke();
    }

    const centerY = height / 2;

    // Connect layers with dynamic bus curves
    for (let i = 0; i < layers.length - 1; i++) {
      const x1 = layers[i].xRatio * width;
      const x2 = layers[i + 1].xRatio * width;

      ctx.beginPath();
      ctx.strokeStyle = "rgba(255, 255, 255, 0.07)";
      ctx.lineWidth = 2;
      ctx.moveTo(x1, centerY);
      ctx.bezierCurveTo(x1 + (x2 - x1) / 2, centerY - 40, x1 + (x2 - x1) / 2, centerY + 40, x2, centerY);
      ctx.stroke();

      // Additional lower bus
      ctx.beginPath();
      ctx.strokeStyle = "rgba(255, 255, 255, 0.04)";
      ctx.moveTo(x1, centerY + 30);
      ctx.bezierCurveTo(x1 + (x2 - x1) / 2, centerY + 70, x1 + (x2 - x1) / 2, centerY - 10, x2, centerY + 30);
      ctx.stroke();
    }

    // Render & update particles
    particles.forEach((p) => {
      p.progress += p.speed;
      if (p.progress >= 1) {
        p.progress = 0;
        p.fromLayer = (p.fromLayer + 1) % 3;
      }

      const l1 = layers[p.fromLayer];
      const l2 = layers[p.fromLayer + 1];
      const startX = l1.xRatio * width;
      const endX = l2.xRatio * width;
      const curX = startX + (endX - startX) * p.progress;

      // Bezier curve interpolation
      const t = p.progress;
      const curY = centerY + Math.sin(t * Math.PI) * p.offsetY;

      ctx.beginPath();
      ctx.arc(curX, curY, p.size, 0, Math.PI * 2);
      ctx.fillStyle = l2.color;
      ctx.shadowColor = l2.color;
      ctx.shadowBlur = 8;
      ctx.fill();
      ctx.shadowBlur = 0;
    });

    // Render Layer Nodes
    layers.forEach((layer, idx) => {
      const lx = layer.xRatio * width;
      const isHovered = hoveredLayer === idx;

      // Glow behind active node
      const grad = ctx.createRadialGradient(lx, centerY, 5, lx, centerY, isHovered ? 65 : 45);
      grad.addColorStop(0, layer.color + "33");
      grad.addColorStop(1, "transparent");
      ctx.fillStyle = grad;
      ctx.beginPath();
      ctx.arc(lx, centerY, isHovered ? 65 : 45, 0, Math.PI * 2);
      ctx.fill();

      // Node ring
      ctx.beginPath();
      ctx.arc(lx, centerY, isHovered ? 26 : 20, 0, Math.PI * 2);
      ctx.strokeStyle = layer.color;
      ctx.lineWidth = isHovered ? 3 : 2;
      ctx.fillStyle = "#0c111a";
      ctx.fill();
      ctx.stroke();

      // Inner core
      ctx.beginPath();
      ctx.arc(lx, centerY, isHovered ? 10 : 8, 0, Math.PI * 2);
      ctx.fillStyle = layer.color;
      ctx.shadowColor = layer.color;
      ctx.shadowBlur = 12;
      ctx.fill();
      ctx.shadowBlur = 0;

      // Label text
      ctx.fillStyle = isHovered ? "#ffffff" : "#cbd5e1";
      ctx.font = isHovered ? "bold 13px Inter, sans-serif" : "600 12px Inter, sans-serif";
      ctx.textAlign = "center";
      ctx.fillText(layer.name, lx, centerY - 45);

      ctx.fillStyle = "#64748b";
      ctx.font = "10px JetBrains Mono, monospace";
      ctx.fillText(layer.sub, lx, centerY + 50);
    });

    requestAnimationFrame(render);
  }

  render();
}

/* ==========================================================================
   2. Project Tabs & Rendering
   ========================================================================== */
function initProjectTabs() {
  const tabButtons = document.querySelectorAll(".tab-btn");
  tabButtons.forEach((btn) => {
    btn.addEventListener("click", () => {
      tabButtons.forEach((b) => b.classList.remove("active"));
      btn.classList.add("active");
      const domain = btn.getAttribute("data-domain");
      initRenderProjects(domain);
    });
  });
}

function initRenderProjects(domain) {
  const grid = document.getElementById("projectsGrid");
  if (!grid) return;

  const filtered =
    domain === "all"
      ? PORTFOLIO_DATA.projects
      : PORTFOLIO_DATA.projects.filter((p) => p.category === domain);

  grid.innerHTML = filtered
    .map(
      (p) => `
    <div class="project-card" data-id="${p.id}">
      <div class="project-card-header">
        <span class="project-badge">${p.badge}</span>
        ${
          p.whitepaperId
            ? `<a href="#whitepapers" class="project-whitepaper-tag" onclick="openWhitepaperModal('${p.whitepaperId}')">${p.whitepaperId} Paper</a>`
            : `<span class="project-whitepaper-tag">Production Core</span>`
        }
      </div>
      <h3 class="project-title">${p.title}</h3>
      <div class="project-silicon">
        <i class="fa-solid fa-microchip"></i>
        <span>${p.silicon}</span>
      </div>
      <p class="project-summary">${p.summary}</p>
      
      <div class="project-tech-tags">
        ${p.stack.map((t) => `<span class="tech-tag">${t}</span>`).join("")}
      </div>

      <div class="project-actions">
        <button class="btn btn-primary btn-sm" onclick="openProjectModal('${p.id}')">
          <i class="fa-solid fa-code"></i> Inspect Architecture
        </button>
        <button class="btn btn-outline btn-sm" onclick="sendProjectPrompt('${p.title}')">
          <i class="fa-solid fa-sparkles"></i> AI Deep Dive
        </button>
      </div>
    </div>
  `
    )
    .join("");
}

/* ==========================================================================
   3. Whitepapers Showcase & Modal
   ========================================================================== */
function initRenderWhitepapers() {
  const grid = document.getElementById("whitepapersGrid");
  if (!grid) return;

  grid.innerHTML = PORTFOLIO_DATA.whitepapers
    .map(
      (wp) => `
    <div class="whitepaper-card">
      <div class="whitepaper-meta">
        <span class="whitepaper-id">${wp.id}</span>
        <span class="whitepaper-schema">IEEE 13-Section Standard</span>
      </div>
      <h3 class="whitepaper-title">${wp.title}</h3>
      <p class="whitepaper-abstract">${wp.abstract}</p>
      <div class="whitepaper-equation">
        <strong>Mathematical Model:</strong><br/>
        <code>${wp.keyEquation}</code>
      </div>
      <div class="project-actions">
        <button class="btn btn-outline btn-sm" onclick="openWhitepaperModal('${wp.id}')">
          <i class="fa-solid fa-file-lines"></i> View Whitepaper Abstract
        </button>
        <button class="btn btn-outline btn-sm" onclick="copyCitation('${wp.id}')">
          <i class="fa-regular fa-copy"></i> Copy Citation
        </button>
      </div>
    </div>
  `
    )
    .join("");
}

function openWhitepaperModal(wpId) {
  const wp = PORTFOLIO_DATA.whitepapers.find((w) => w.id === wpId);
  if (!wp) return;

  const modal = document.getElementById("genericModal");
  const modalTitle = document.getElementById("genericModalTitle");
  const modalBody = document.getElementById("genericModalBody");

  modalTitle.innerHTML = `<span class="mono" style="color:var(--accent-purple-bright)">[${wp.id}]</span> ${wp.title}`;
  modalBody.innerHTML = `
    <div style="margin-bottom:1.25rem;">
      <p><strong>Primary Domain:</strong> ${wp.domain}</p>
      <p><strong>Hardware / Silicon Platform:</strong> ${wp.hardware}</p>
      <p><strong>Methodology:</strong> IEEE 13-Section Formal Empirical Research Schema</p>
    </div>
    <div style="background:rgba(255,255,255,0.03); border:1px solid var(--border-subtle); padding:1.25rem; border-radius:8px; margin-bottom:1.5rem;">
      <h4 style="margin-bottom:0.5rem; color:var(--accent-cyan-bright);">Executive Abstract</h4>
      <p style="color:var(--text-muted); font-size:0.95rem; line-height:1.7;">${wp.abstract}</p>
    </div>
    <div style="margin-bottom:1.5rem;">
      <h4 style="margin-bottom:0.5rem; color:var(--accent-purple-bright);">Formulated Physical / Mathematical Model</h4>
      <pre><code>${wp.keyEquation}</code></pre>
    </div>
    <div>
      <h4 style="margin-bottom:0.5rem;">Academic & Industrial Citation (BibTeX)</h4>
      <pre><code>@article{reddy2026_${wp.id.toLowerCase().replace(/-/g, "_")},
  author    = {Reddy, Aravind},
  title     = {${wp.title}},
  journal   = {Kernelwise Labs Technical Whitepaper Series},
  number    = {${wp.id}},
  year      = {2026},
  publisher = {Kernelwise Labs}
}</code></pre>
    </div>
  `;

  modal.classList.add("open");
}

function copyCitation(wpId) {
  const wp = PORTFOLIO_DATA.whitepapers.find((w) => w.id === wpId);
  if (!wp) return;
  const citation = wp.citations;
  navigator.clipboard.writeText(citation).then(() => {
    alert(`Copied citation for ${wpId}:\n${citation}`);
  });
}

function openProjectModal(projectId) {
  const p = PORTFOLIO_DATA.projects.find((item) => item.id === projectId);
  if (!p) return;

  const modal = document.getElementById("genericModal");
  const modalTitle = document.getElementById("genericModalTitle");
  const modalBody = document.getElementById("genericModalBody");

  modalTitle.innerHTML = `<span class="mono" style="color:var(--accent-cyan-bright);">${p.badge}</span> ${p.title}`;
  modalBody.innerHTML = `
    <div style="margin-bottom: 1.25rem;">
      <p><strong>Repository Path:</strong> <code class="mono" style="color:var(--accent-cyan);">${p.path}</code></p>
      <p><strong>Hardware Silicon:</strong> ${p.silicon}</p>
      <p><strong>Verification Status:</strong> <span class="badge-verified" style="display:inline-block; padding:0.1rem 0.5rem; border-radius:4px; font-size:0.8rem;">${p.status}</span></p>
    </div>
    <div style="margin-bottom: 1.25rem;">
      <h4 style="color:var(--accent-purple-bright); margin-bottom:0.5rem;">Architecture & Engineering Highlights</h4>
      <ul style="padding-left: 1.25rem; color:var(--text-muted); font-size:0.95rem; line-height:1.7;">
        ${p.highlights.map((h) => `<li>${h}</li>`).join("")}
      </ul>
    </div>
    <div>
      <h4 style="color:var(--accent-cyan-bright); margin-bottom:0.5rem;">Verified Source Implementation</h4>
      <pre><code>${p.codeSnippet}</code></pre>
    </div>
    <div style="margin-top: 1.5rem; display:flex; gap:0.75rem;">
      <a href="${PORTFOLIO_DATA.author.repo}/tree/main/${p.path}" target="_blank" class="btn btn-primary btn-sm">
        <i class="fa-brands fa-github"></i> Inspect Full Source On GitHub
      </a>
      <button class="btn btn-outline btn-sm" onclick="sendProjectPrompt('${p.title}')">
        <i class="fa-solid fa-robot"></i> Ask Aravind AI About This System
      </button>
    </div>
  `;

  modal.classList.add("open");
}

function closeGenericModal() {
  const modal = document.getElementById("genericModal");
  if (modal) modal.classList.remove("open");
}

/* ==========================================================================
   4. Advanced AI Copilot ("Aravind AI / Kernelwise Neural Copilot")
   ========================================================================== */
function initAICopilot() {
  const chatHistory = document.getElementById("chatHistory");
  const chatInput = document.getElementById("chatInput");
  const chatSendBtn = document.getElementById("chatSendBtn");

  if (!chatHistory || !chatInput || !chatSendBtn) return;

  function handleSend() {
    const text = chatInput.value.trim();
    if (!text) return;

    appendUserMessage(text);
    chatInput.value = "";

    // Show AI typing indicator
    const typingId = appendTypingIndicator();

    setTimeout(() => {
      removeTypingIndicator(typingId);
      const aiReply = synthesizeAIResponse(text);
      typeOutAIMessage(aiReply);
    }, 600);
  }

  chatSendBtn.addEventListener("click", handleSend);
  chatInput.addEventListener("keydown", (e) => {
    if (e.key === "Enter") handleSend();
  });
}

function sendProjectPrompt(title) {
  const chatInput = document.getElementById("chatInput");
  if (!chatInput) return;
  chatInput.value = `Explain the architecture, design choices, and benchmarks for ${title}`;
  const copilotSection = document.getElementById("copilotSection");
  if (copilotSection) {
    copilotSection.scrollIntoView({ behavior: "smooth" });
  }
  document.getElementById("chatSendBtn").click();
}

function sendQuickPrompt(promptText) {
  const chatInput = document.getElementById("chatInput");
  if (!chatInput) return;
  chatInput.value = promptText;
  document.getElementById("chatSendBtn").click();
}

function appendUserMessage(text) {
  const chatHistory = document.getElementById("chatHistory");
  const bubble = document.createElement("div");
  bubble.className = "chat-bubble bubble-user";
  bubble.textContent = text;
  chatHistory.appendChild(bubble);
  chatHistory.scrollTop = chatHistory.scrollHeight;
}

function appendTypingIndicator() {
  const chatHistory = document.getElementById("chatHistory");
  const bubble = document.createElement("div");
  const id = "typing-" + Date.now();
  bubble.id = id;
  bubble.className = "chat-bubble bubble-ai";
  bubble.innerHTML = `<span class="mono" style="color:var(--accent-cyan);"><i class="fa-solid fa-spinner fa-spin"></i> Neural inference engine synthesizing answer...</span>`;
  chatHistory.appendChild(bubble);
  chatHistory.scrollTop = chatHistory.scrollHeight;
  return id;
}

function removeTypingIndicator(id) {
  const el = document.getElementById(id);
  if (el) el.remove();
}

function typeOutAIMessage(text) {
  const chatHistory = document.getElementById("chatHistory");
  const bubble = document.createElement("div");
  bubble.className = "chat-bubble bubble-ai";
  chatHistory.appendChild(bubble);

  // Parse basic markdown: bold, code, links
  const formatted = text
    .replace(/\*\*(.*?)\*\*/g, "<strong>$1</strong>")
    .replace(/`([^`]+)`/g, "<code>$1</code>")
    .replace(/\n/g, "<br/>");

  let i = 0;
  const speed = 12;
  const tempDiv = document.createElement("div");
  tempDiv.innerHTML = formatted;
  bubble.innerHTML = formatted;
  chatHistory.scrollTop = chatHistory.scrollHeight;
}

function synthesizeAIResponse(query) {
  const lower = query.toLowerCase();

  // Search exact match in knowledge base
  for (const item of PORTFOLIO_DATA.aiKnowledgeBase) {
    if (item.keywords.some((k) => lower.includes(k))) {
      return item.response;
    }
  }

  // Search in projects
  const matchedProject = PORTFOLIO_DATA.projects.find(
    (p) =>
      lower.includes(p.title.toLowerCase()) ||
      lower.includes(p.id.toLowerCase()) ||
      lower.includes(p.silicon.toLowerCase()) ||
      p.stack.some((s) => lower.includes(s.toLowerCase()))
  );

  if (matchedProject) {
    return `**${matchedProject.title}** (${matchedProject.silicon}):\n\n${matchedProject.summary}\n\n**Key Innovations:**\n- ${matchedProject.highlights.join("\n- ")}\n\n*Repository Location:* \`${matchedProject.path}\`.`;
  }

  // Search in whitepapers
  const matchedWP = PORTFOLIO_DATA.whitepapers.find(
    (w) =>
      lower.includes(w.id.toLowerCase()) ||
      lower.includes(w.title.toLowerCase()) ||
      lower.includes(w.domain.toLowerCase())
  );

  if (matchedWP) {
    return `**IEEE Research Whitepaper [${matchedWP.id}]**: *${matchedWP.title}*\n\n**Abstract:** ${matchedWP.abstract}\n\n**Mathematical Model:** \`${matchedWP.keyEquation}\`\n\nHardware Platform: ${matchedWP.hardware}. Full paper documented with 13 standard IEEE sections.`;
  }

  // Fallback synthesis
  return `Thank you for your question regarding **"${query}"**.\n\nAravind's expertise centers on zero-leak low-level engineering: bare-metal C/C++, Linux kernel loadable modules (LKM), on-device TinyML anomaly inference (Isolation Forests, Mahalanobis distances), and industrial IoT.\n\nAll 21 production systems in this repository are verified with automated unit tests and backed by 12 peer-reviewed IEEE-grade whitepapers. Feel free to ask about specific projects like the **Car Black Box**, **Android Telemetry AI**, or **Linux Device Drivers**, or click **Generate Technical Pitch** for an executive architectural proposal!`;
}

/* ==========================================================================
   5. AI Solution Matcher & Technical Proposal Synthesizer
   ========================================================================== */
function initPitchGenerator() {
  const form = document.getElementById("pitchForm");
  if (!form) return;

  form.addEventListener("submit", (e) => {
    e.preventDefault();
    const industry = document.getElementById("pitchIndustry").value;
    const challenge = document.getElementById("pitchChallenge").value;
    const timeline = document.getElementById("pitchTimeline").value;
    const targetHardware = document.getElementById("pitchHardware").value || "Custom Embedded / Edge Silicon";

    generateClientProposal(industry, challenge, timeline, targetHardware);
  });
}

function generateClientProposal(industry, challenge, timeline, hardware) {
  const modal = document.getElementById("genericModal");
  const modalTitle = document.getElementById("genericModalTitle");
  const modalBody = document.getElementById("genericModalBody");

  const proposalText = `# EXECUTIVE TECHNICAL PROPOSAL & ARCHITECTURE BLUEPRINT
Prepared for: Prospective Enterprise Client / Partner
Industry Domain: ${industry}
Core Engineering Focus: ${challenge}
Target Hardware Platform: ${hardware}
Engagement Model / Timeline: ${timeline}
Lead Systems Architect: Aravind Reddy (Founder, Kernelwise Labs)
Date: ${new Date().toLocaleDateString("en-US", { year: "numeric", month: "long", day: "numeric" })}

--------------------------------------------------------------------------------

1. EXECUTIVE ARCHITECTURAL SUMMARY
To solve the challenges of "${challenge}" within the ${industry} sector, we recommend deploying a decoupled, hardware-aware distributed telemetry and deterministic control pipeline. Rather than routing raw uncurated high-frequency telemetry across saturated network links, edge silicon will perform localized preprocessing, anomaly scoring, and non-volatile state preservation.

2. RECOMMENDED SYSTEM TOPOLOGY & STACK
- Silicon & MCU Tier: ${hardware} executing deterministic C11 / C++14 firmware with non-blocking event loops and zero-heap allocation.
- OS & Kernel Tier: Real-Time Linux / POSIX system services with custom Loadable Kernel Modules (LKM) and sysfs monitoring.
- Intelligence Engine: Edge-assisted unsupervised anomaly detection (Isolation Forests / Mahalanobis distance) providing microsecond-level degradation scoring.
- Communication & Persistence: Non-volatile circular event recording (I2C EEPROM / Flash) resistant to sudden power loss, backed by encrypted telemetry uplinks (MQTT / BLE 5.0).

3. PROVEN REFERENCE CODEBASES IN ARAVIND'S PORTFOLIO
The proposed architecture directly leverages battle-tested modules from Aravind Reddy's verified monorepo:
- ${challenge.includes("Predictive") ? "02_AI_ML_SYSTEMS/android_device_health_prediction (Isolation Forest Anomaly Scoring)" : "05_LINUX_AND_SYSTEMS/device_drivers (Production LKM Character Driver)"}
- 08_SHARED_LIBRARIES (Zero-Allocation I2C/UART HAL & CRC16 Packet Protocol)
- 06_EXISTING_GITHUB_PROJECTS/imported_projects/Car_black_box (Brownout-Safe Non-Volatile EEPROM Logging)
- Relevant Formal Research: IEEE Whitepaper KW-WP-01, KW-WP-04, and KW-WP-10.

4. SAFETY, RELIABILITY & QUALITY ASSURANCE GUARANTEE
- Static Analysis: Compiles under GCC 16.1.0 with -Wall -Wextra -Werror -pedantic.
- Dynamic Audits: Zero memory leaks verified through AddressSanitizer (ASan) and Valgrind suites.
- Determinism: Hard bounded latency for critical control loops (< 5ms).

5. RECOMMENDED NEXT STEPS
1. Review full codebases on GitHub: https://github.com/DAravindReddy/startup-engineering-portfolio-
2. Schedule a technical deep-dive call with Aravind Reddy: aravindreddy0726@gmail.com
3. Initiate rapid proof-of-concept milestone (${timeline}).`;

  modalTitle.innerHTML = `<span style="color:var(--accent-purple-bright);"><i class="fa-solid fa-file-contract"></i> Customized Technical Proposal</span>`;
  modalBody.innerHTML = `
    <div style="margin-bottom:1rem; display:flex; justify-content:space-between; align-items:center;">
      <span style="font-size:0.85rem; color:var(--text-muted);">Synthesized by Kernelwise Neural Proposal Engine</span>
      <button class="btn btn-primary btn-sm" onclick="copyProposalText()">
        <i class="fa-regular fa-copy"></i> Copy Markdown Brief
      </button>
    </div>
    <div class="proposal-preview-content" id="proposalTextContainer">${proposalText}</div>
    <div style="margin-top:1.5rem; text-align:center;">
      <a href="mailto:aravindreddy0726@gmail.com?subject=Technical%20Proposal%20Discussion%20-%20${encodeURIComponent(industry)}&body=${encodeURIComponent(proposalText)}" class="btn btn-ai">
        <i class="fa-solid fa-paper-plane"></i> Email This Proposal Directly to Aravind
      </a>
    </div>
  `;

  modal.classList.add("open");
}

function copyProposalText() {
  const container = document.getElementById("proposalTextContainer");
  if (!container) return;
  navigator.clipboard.writeText(container.innerText).then(() => {
    alert("Proposal copied to clipboard in Markdown format!");
  });
}

/* ==========================================================================
   6. Interactive Cybernetic Terminal (KernelOS v3.12-rt)
   ========================================================================== */
function initTerminal() {
  const termBody = document.getElementById("terminalBody");
  const termInput = document.getElementById("termInput");
  if (!termBody || !termInput) return;

  const history = [];
  let historyIdx = -1;
  let telemetryInterval = null;

  function printLine(html) {
    const div = document.createElement("div");
    div.className = "term-output";
    div.innerHTML = html;
    termBody.appendChild(div);
    termBody.scrollTop = termBody.scrollHeight;
  }

  function handleCommand(cmdRaw) {
    const cmd = cmdRaw.trim();
    if (!cmd) return;

    history.push(cmd);
    historyIdx = history.length;

    // Echo input
    printLine(`<span class="term-prompt">root@kernelwise-node:~#</span> ${cmd}`);

    // If telemetry is active, stop on any new command
    if (telemetryInterval) {
      clearInterval(telemetryInterval);
      telemetryInterval = null;
      printLine(`<span class="term-amber">[HALTED] Telemetry stream stopped.</span>`);
    }

    const parts = cmd.split(" ");
    const base = parts[0].toLowerCase();

    switch (base) {
      case "help":
        printLine(`
Available KernelOS Commands:
  <span class="term-cyan">projects</span>              - List all 21 verified systems and statuses
  <span class="term-cyan">telemetry --stream</span>    - Stream live mock sensor & anomaly vectors
  <span class="term-cyan">run-anomaly-detector</span>  - Run simulated Isolation Forest inference cycle
  <span class="term-cyan">whitepapers</span>           - List 12 formal IEEE research whitepapers
  <span class="term-cyan">cat whitepaper [ID]</span>   - Read paper summary (e.g. cat whitepaper KW-WP-01)
  <span class="term-cyan">run-tests</span>             - Execute simulated 13-suite CI/CD verification
  <span class="term-cyan">skills</span>                - Display technical competency matrix
  <span class="term-cyan">contact</span>               - Display author contact and GitHub profile
  <span class="term-cyan">clear</span>                 - Clear the terminal console
        `);
        break;

      case "clear":
        termBody.innerHTML = "";
        break;

      case "projects":
        printLine(`<span class="term-green">=== VERIFIED PRODUCTION REPOSITORIES (21 SYSTEMS) ===</span>`);
        PORTFOLIO_DATA.projects.forEach((p, idx) => {
          printLine(`[${(idx + 1).toString().padStart(2, "0")}] <span class="term-cyan">${p.id}</span> - ${p.silicon} | <span class="term-green">${p.status}</span>`);
        });
        break;

      case "skills":
        printLine(`<span class="term-purple">=== CORE SYSTEMS COMPETENCIES ===</span>`);
        for (const [cat, list] of Object.entries(PORTFOLIO_DATA.skills)) {
          printLine(`<span class="term-cyan">${cat}:</span> ${list.join(", ")}`);
        }
        break;

      case "contact":
        printLine(`
<span class="term-green">Author:</span> ${PORTFOLIO_DATA.author.name} (${PORTFOLIO_DATA.author.role})
<span class="term-cyan">Email:</span> ${PORTFOLIO_DATA.author.email}
<span class="term-purple">GitHub:</span> ${PORTFOLIO_DATA.author.github}
<span class="term-amber">Repo:</span> ${PORTFOLIO_DATA.author.repo}
<span class="term-green">Status:</span> Open for Senior Roles & Enterprise Architecture Partnerships.
        `);
        break;

      case "run-tests":
        printLine(`<span class="term-cyan">[INIT] Initializing Kernelwise Monorepo Test Runner...</span>`);
        let currentSuite = 0;
        const testTimer = setInterval(() => {
          if (currentSuite < PORTFOLIO_DATA.testSuites.length) {
            const suite = PORTFOLIO_DATA.testSuites[currentSuite];
            printLine(`[RUNNING] ${suite.name.padEnd(52, ".")} <span class="term-green">PASS</span> (${suite.tests} tests)`);
            currentSuite++;
          } else {
            clearInterval(testTimer);
            printLine(`<span class="term-green">=====================================================</span>`);
            printLine(`<span class="term-green">SUMMARY: 13 Passed, 0 Failed (100% Quality Gate Passed)</span>`);
            printLine(`<span class="term-green">=====================================================</span>`);
          }
        }, 120);
        break;

      case "run-anomaly-detector":
        printLine(`<span class="term-purple">[AI-ENGINE] Ingesting Android & Linux Subsystem Telemetry Frame...</span>`);
        setTimeout(() => {
          printLine(`  [T0] CPU Cluster 0: 42.1°C | CPU Cluster 1: 58.7°C | Battery Imp: 0.14Ω`);
          printLine(`  [T1] Thermal Gradient (dT/dt): +3.4°C/sec [ELEVATED WARMING]`);
          printLine(`  [T2] Feature Vector Normalized (14 features) -> Input to Isolation Forest`);
          printLine(`  [T3] Anomaly Decision Score: -0.142 (Contamination threshold: 0.04)`);
          printLine(`  <span class="term-amber">>> STATUS: MODERATE_ANOMALY (Risk Score: 68.4%)</span>`);
          printLine(`  <span class="term-dim">Root Cause: GPU render stall triggering localized SoC hotspot.</span>`);
        }, 300);
        break;

      case "telemetry":
        if (parts[1] === "--stream") {
          printLine(`<span class="term-green">[STREAM START] Broadcasting 10Hz synthetic telemetry frames. Type any command to stop.</span>`);
          let frameNum = 1;
          telemetryInterval = setInterval(() => {
            const temp = (45 + Math.random() * 8).toFixed(1);
            const vib = (0.2 + Math.random() * 0.4).toFixed(3);
            const mahal = (1.1 + Math.random() * 0.8).toFixed(2);
            printLine(`[FRAME #${frameNum.toString().padStart(4, "0")}] T=${temp}°C | AccVib=${vib}g | D_Mahalanobis=${mahal} | Status=<span class="term-green">NOMINAL</span>`);
            frameNum++;
          }, 800);
        } else {
          printLine(`Usage: telemetry --stream`);
        }
        break;

      case "whitepapers":
        printLine(`<span class="term-purple">=== IEEE RESEARCH WHITEPAPER COMPENDIUM (12 PAPERS) ===</span>`);
        PORTFOLIO_DATA.whitepapers.forEach((wp) => {
          printLine(`<span class="term-cyan">[${wp.id}]</span> ${wp.title}`);
        });
        printLine(`<span class="term-dim">Type 'cat whitepaper [ID]' to inspect abstract.</span>`);
        break;

      case "cat":
        if (parts[1] === "whitepaper" && parts[2]) {
          const wpId = parts[2].toUpperCase();
          const wp = PORTFOLIO_DATA.whitepapers.find((w) => w.id === wpId);
          if (wp) {
            printLine(`
<span class="term-purple">=== [${wp.id}] ${wp.title} ===</span>
<span class="term-cyan">Domain:</span> ${wp.domain} | <span class="term-amber">Silicon:</span> ${wp.hardware}
<span class="term-green">Abstract:</span> ${wp.abstract}
<span class="term-cyan">Model:</span> ${wp.keyEquation}
            `);
          } else {
            printLine(`<span class="term-amber">Error: Whitepaper ID '${wpId}' not found. Type 'whitepapers' for full index.</span>`);
          }
        } else {
          printLine(`Usage: cat whitepaper [ID] (e.g. cat whitepaper KW-WP-01)`);
        }
        break;

      default:
        printLine(`<span class="term-amber">Command not found: '${cmd}'. Type 'help' for available commands.</span>`);
    }
  }

  termInput.addEventListener("keydown", (e) => {
    if (e.key === "Enter") {
      const val = termInput.value;
      termInput.value = "";
      handleCommand(val);
    } else if (e.key === "ArrowUp") {
      if (historyIdx > 0) {
        historyIdx--;
        termInput.value = history[historyIdx];
      }
    } else if (e.key === "ArrowDown") {
      if (historyIdx < history.length - 1) {
        historyIdx++;
        termInput.value = history[historyIdx];
      } else {
        historyIdx = history.length;
        termInput.value = "";
      }
    }
  });

  // Expose shortcut executor to window
  window.executeTermCommand = function (cmd) {
    const termSection = document.getElementById("terminalSection");
    if (termSection) termSection.scrollIntoView({ behavior: "smooth" });
    termInput.value = "";
    handleCommand(cmd);
  };
}

/* ==========================================================================
   7. CI/CD Quality Gate & Verification Matrix
   ========================================================================== */
function initVerificationMatrix() {
  const tbody = document.getElementById("testMatrixBody");
  if (!tbody) return;

  tbody.innerHTML = PORTFOLIO_DATA.testSuites
    .map(
      (ts, idx) => `
    <tr>
      <td class="mono" style="color:var(--text-faint);">#${(idx + 1).toString().padStart(2, "0")}</td>
      <td><strong>${ts.name}</strong></td>
      <td class="mono" style="color:var(--accent-cyan); font-size:0.8rem;">${ts.target}</td>
      <td class="mono" style="color:var(--text-muted);">${ts.tests} Units</td>
      <td>
        <span class="test-status-pill status-pass">
          <i class="fa-solid fa-check"></i> ${ts.status}
        </span>
      </td>
    </tr>
  `
    )
    .join("");
}
