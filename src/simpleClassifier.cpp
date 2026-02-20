#include "simpleClassifier.h"

SimpleClassifier::SimpleClassifier() {
    initRules();
}

void SimpleClassifier::initRules() {
    // Each rule: case-insensitive regex pattern -> category
    // Order matters: first match wins

    // --- Supermercados ---
    m_rules.append({QRegularExpression("lider|walmart|unimarc|liquimax|laicao|santa\\*isabel|sta\\*isabel|homecenter|hiper|(?<!ali)\\bexpress\\b|jumbo|tottus|(santa|sta)\\s*isabel|esencia\\s*vegana|ekono|mayorista\\s*10", QRegularExpression::CaseInsensitiveOption), "groceries"});

    // --- Farmacias ---
    m_rules.append({QRegularExpression("farmacias?\\s*(ahumada|cruz\\s*verde|salcobrand|knop|dr\\.?\\s*simi)|fasa|cruzverde", QRegularExpression::CaseInsensitiveOption), "drugstore"});

    // --- Restaurantes / Comida ---
    m_rules.append({QRegularExpression("uber\\s*eats|mcdonald|burger\\s*king|starbucks|papa\\s*john|domino|Dunkin|subway|kfc|juan\\s*maestro|tarragona|pizza|rappi|pedidos\\s*ya|uber\\s*eats|cornershop|ifood|doggis|restauran", QRegularExpression::CaseInsensitiveOption), "delivery/eating out"});

    // --- Transporte ---
    m_rules.append({QRegularExpression("efe|uber|cabify|didi|beat|Condor|tur\\s*bus|bip!?|metro\\s*s\\.?a|recarga\\s*bip", QRegularExpression::CaseInsensitiveOption), "transport"});

    // --- Combustible ---
    m_rules.append({QRegularExpression("copec|shell|petrobras|enex|gasolinera|combustible|estacion\\s*de\\s*servicio", QRegularExpression::CaseInsensitiveOption), "gas"});

    // --- Servicios basicos ---
    m_rules.append({QRegularExpression("enel|chilquinta|cge|aguas\\s*andinas|esval|essbio|entel|movistar|Telefónica|claro|wom|vtr|mundo\\s*pacifico|gtd|telsur|metrogas|gas\\s*natural|lipigas|abastible", QRegularExpression::CaseInsensitiveOption), "utilities"});

    // --- Salud ---
    m_rules.append({QRegularExpression("clinica|hospital|isapre|fonasa|colmena|consalud|banmedica|vida\\s*tres|megasalud|integramedica|red\\s*salud|dental|optica|laboratorio", QRegularExpression::CaseInsensitiveOption), "healtcare"});

    // --- Educacion ---
    m_rules.append({QRegularExpression("universidad|colegio|instituto|escuela|jardin\\s*infantil|duoc|inacap|aiep|matricula|arancel|mensualidad\\s*escolar", QRegularExpression::CaseInsensitiveOption), "education"});

    // --- Entretenimiento ---
    m_rules.append({QRegularExpression("netflix|spotify|disney|hbo|amazon\\s*prime|youtube\\s*premium|steam|playstation|xbox|nintendo|cineplanet|cinemark|cinepolis|hoyts|apple\\.com|google\\s*play", QRegularExpression::CaseInsensitiveOption), "subscription"});

    // --- Compras online ---
    m_rules.append({QRegularExpression("aliexpress|alibaba|\\btemu\\b|\\bshein\\b|\\bwish\\b|banggood|gearbest|dhgate", QRegularExpression::CaseInsensitiveOption), "online shopping"});

    // --- Retail / Tiendas ---
    m_rules.append({QRegularExpression("rosen|DECATHLON|hym|pc\\s*Factory|falabella|ripley|paris|la\\s*polar|hites|abcdin|sodimac|homecenter|easy|construmart|imperial|corona|ikea|amazon|mercadolibre|merpago|mercado\\s*pago", QRegularExpression::CaseInsensitiveOption), "retail"});

    // --- Vestimenta ---
    m_rules.append({QRegularExpression("flores|\\bzara\\b|h&m|forever\\s*21|\\bnike\\b|\\badidas\\b|\\bpuma\\b|\\bbata\\b|hush\\s*puppies|tricot|fashion", QRegularExpression::CaseInsensitiveOption), "clothes"});

    // --- Seguros ---
    m_rules.append({QRegularExpression("seguro|mapfre|liberty|bci\\s*seguros|chilena\\s*consolidada|metlife|zurich|hdi", QRegularExpression::CaseInsensitiveOption), "insurance"});

    // --- Abono / Deposito generico ---
    m_rules.append({QRegularExpression("abono|dep[oó]sito|devoluci[oó]n|reembolso", QRegularExpression::CaseInsensitiveOption), "deposit"});

    // --- Transferencias ---
    m_rules.append({QRegularExpression("transf|transferencia", QRegularExpression::CaseInsensitiveOption), "bank transfer"});

    // --- Pago tarjeta ---
    m_rules.append({QRegularExpression("pago\\s*tarjeta|pago\\s*tc|pago.*cr[eé]dito", QRegularExpression::CaseInsensitiveOption), "card payment"});

    // --- Cajero automatico ---
    m_rules.append({QRegularExpression("giro.*cajero|cajero\\s*autom[aá]tico|atm|redbanc", QRegularExpression::CaseInsensitiveOption), "withdraw cash"});

    // --- Comisiones ---
    m_rules.append({QRegularExpression("comisi[oó]n|comisiones|amortizacion|impuesto|inter[eé]ses|cargo\\s*mantenci[oó]n|costo\\s*mantenci[oó]n", QRegularExpression::CaseInsensitiveOption), "bank comission"});

    // --- Sueldo / Remuneraciones ---
    m_rules.append({QRegularExpression("sueldo|remuneraci[oó]n|honorario|n[oó]mina|liquidaci[oó]n\\s*sueldo|proveedor", QRegularExpression::CaseInsensitiveOption), "paycheck"});

     // --- Inversiones ---
    m_rules.append({QRegularExpression("inversi[oón]\\es", QRegularExpression::CaseInsensitiveOption), "investment"});

    // --- Pago en linea ---
    m_rules.append({QRegularExpression("pago\\s*en\\s*linea|pago.*internet|webpay|servipag|sencillito|caja\\s*vecina", QRegularExpression::CaseInsensitiveOption), "online payment"});

    // --- Compras genericas ---
    m_rules.append({QRegularExpression("compra", QRegularExpression::CaseInsensitiveOption), "purchase"});

}

QString SimpleClassifier::classify(const QString& description) const {
    for (const Rule& rule : m_rules) {
        if (rule.pattern.match(description).hasMatch()) {
            return rule.category;
        }
    }
    return "others";
}
