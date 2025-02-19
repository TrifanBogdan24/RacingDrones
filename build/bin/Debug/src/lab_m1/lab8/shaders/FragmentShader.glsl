#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 light_position2;
uniform vec3 light_color2;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform int type_of_light;
uniform float cut_off_angle;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    // Implementarea vectorilor
    vec3 N = normalize(world_normal);
    vec3 L1 = normalize(light_position - world_position);
    vec3 L2 = normalize(light_position2 - world_position);  // vectorul de fragment catre cea de a doua sursa de lumina
    vec3 V = normalize(eye_position - world_position);
    vec3 H1 = normalize(L1 + V);
    vec3 H2 = normalize(L2 + V);

    // Iluminarea ambientala
    float ambient_light = 0.25;  // intensitatea luminii ambientale
    vec3 ambient = ambient_light * object_color;  // componenta ambientala a luminii

    // Iluminarea difuza si speculara pentru prima lumina
    float diffuse1 = material_kd * max(dot(N, L1), 0.0);  // componenta difuza a primei lumini
    float specular1 = 0.0;  // reflexia speculara
    if (diffuse1 > 0.0) {
        specular1 = material_ks * pow(max(dot(N, H1), 0.0), material_shininess);
    }
    vec3 light1_contribution = (diffuse1 + specular1) * vec3(1.0);  // combinatia componentelor difuza si speculara

    // Iluminarea difuza si speculara pentru a doua lumina
    float diffuse2 = material_kd * max(dot(N, L2), 0.0);
    float specular2 = 0.0;
    if (diffuse2 > 0.0) {
        specular2 = material_ks * pow(max(dot(N, H2), 0.0), material_shininess);
    }
    vec3 light2_contribution = (diffuse2 + specular2) * light_color2;

    // Calcularea spotlight
    vec3 total_light;
    if (type_of_light == 1) {
    // Spotlight pentru prima lumina
    float cut_off_rad = radians(cut_off_angle);
    float spot_effect1 = dot(-L1, light_direction);
    float spot_light_limit = cos(cut_off_rad);

    vec3 light1_spot = vec3(0.0);
    if (spot_effect1 > spot_light_limit) {
        float linear_att1 = (spot_effect1 - spot_light_limit) / (1.0 - spot_light_limit);
        float attenuation1 = pow(linear_att1, 2.0);
        light1_spot = attenuation1 * light1_contribution;
    }

    // Spotlight pentru a doua lumina
    float spot_effect2 = dot(-L2, light_direction); // Folosim aceeasi light_direction
    vec3 light2_spot = vec3(0.0);
    if (spot_effect2 > spot_light_limit) {
        float linear_att2 = (spot_effect2 - spot_light_limit) / (1.0 - spot_light_limit);
        float attenuation2 = pow(linear_att2, 2.0);
        light2_spot = attenuation2 * light2_contribution;
    }

    // Combina doar lumina spotlight
    total_light = ambient + light1_spot + light2_spot;

} else {
    // Adauga contributiile standard (ambientala, difuza, speculara)
    total_light = ambient + light1_contribution + light2_contribution;
}

    // Culoarea finala
    out_color = vec4(total_light * object_color, 1.0);
}
